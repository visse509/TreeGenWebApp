#include "tree.h"
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <algorithm>
#include <windows.h> 
#pragma managed

const double PI = 3.1415926;
static int extrusions = 0;
static int totalBranches = 0;
Tree::Tree(int _nEdges,int _branchLevel,std::string propXMl,std::string lsXml)
{
    branchLevel = _branchLevel;
	int nEdges = _nEdges;
	prop = TreeProperty::getInstance(propXMl);
	stringGenerator *sG = new stringGenerator(lsXml);
	if(_nEdges == -1){
		nEdges = prop->nEdges[branchLevel];
	}

	charList = sG->getString();
	trunk = new cylinder(nEdges,(float)1/(branchLevel+1),prop->nSegments[branchLevel]);
    treeLevel = 0;
    branchChance = 0;
    n_branches = 0;
    branches = std::vector<Tree*>();
	leafs = std::vector<Leaf*>();
    upVector = new Vert(0,1,0);
	heightOnTree = 0;
}
Tree::Tree(int nEdges,int _branchLevel)
{
    branchLevel = _branchLevel;
	prop = TreeProperty::getInstance("treeProp.xml");
	stringGenerator *sG = new stringGenerator("treeString.xml");
	
	charList = sG->getString();
	trunk = new cylinder(nEdges,(float)1/(branchLevel+1),prop->nSegments[branchLevel]);
    treeLevel = 0;
    branchChance = 0;
    n_branches = 0;
    branches = std::vector<Tree*>();
	leafs = std::vector<Leaf*>();
    upVector = new Vert(0,1,0);
	heightOnTree = 0;
}

void Tree::extrudeTrunk(){
	int segments = prop->nSegments[branchLevel];
	float length = prop->lengths[branchLevel];
	Vert *temp = getBranchRotation(this);
	trunk->extrude(0.93f,length,temp);
}

std::vector<char> Tree::parseString(){
	std::vector<char> res;
	std::ifstream is("treeString.txt"); 
	 while (is.good())          // loop while extraction from file is possible
	{
		char c = is.get();       // get character from file
		if (is.good()){
			res.push_back(c);
		}
	}
	 return res;
}

void Tree::createTreeLS(){
	Tree* currentTree = this;
	std::vector<Tree*> treeStack;
	int length = charList.size();
	for(int i = 0; i < charList.size(); i++){
		char c = charList[i];
		if(c == 'f'){
			currentTree->extrudeTrunk();
		}
		else if((c == 'l') || (c == 'r')){
			int t1 = currentTree->trunk->getNEdges();
			int t2 = currentTree->trunk->getNLevels();
			int pos = (currentTree->trunk->getNEdges())*(currentTree->trunk->getNLevels()-1) + ((currentTree->trunk->getNEdges())/2);
			if(c == 'l'){
				pos -= ((currentTree->trunk->getNEdges())/4);
			}
			else if(c == 'r'){
				pos += ((currentTree->trunk->getNEdges())/4) + 1;
			}
			currentTree = currentTree->init_branch(prop->nEdges[branchLevel+1],pos);
			currentTree->createLeaf();
			currentTree->createLeaf();
			currentTree->createLeaf();
			totalBranches++;
		}
		else if(c == '['){
			treeStack.push_back(currentTree);
		}else if(c == ']'){
			currentTree = treeStack[treeStack.size()-1];
			treeStack.pop_back();
		}
	}
}

void Tree::createTree(){
	for(int i = 0; i <  this->prop->nSegments[branchLevel];i++){
		this->extrudeTrunk();
	}
	for(int i = 0; i < this->prop->nBranchesList[branchLevel];i++){
		this->createBranch(branchLevel+1);
	//	this->branches[i]->createLeaf();
	}
	for(int i = 0; i < this->branches.size();i++){
		this->branches[i]->createTree();
	}

}
void Tree::createBranchRecursive(int branchLevels){
	init_branch(5,1);
}

void Tree::createBranch(int branchLevel){
	Tree* newBranch = init_branch(prop->nEdges[branchLevel],getRandPos());
	totalBranches++;
}

int Tree::getRandPos(){

	return rand() % (3*(this->trunk->getNEdges()*this->trunk->getNLevels())/4) + ((this->trunk->getNEdges()*this->trunk->getNLevels())/4);
}


Tree* Tree::init_branch(int nEdges,int pos){
    Vert *v1,*v2,*v3,*v4;
    std::vector<int> vertList = trunk->getVertexIndexFromPlane(pos);
	Tree *tree = new Tree(nEdges,branchLevel+1);
	tree->setHeightPosition(this,pos);
	branches.push_back(tree);
	tree->branchLevel = branchLevel+1;
	v1 = trunk->getMeshData()->vertexList[vertList[0]];
    v2 = trunk->getMeshData()->vertexList[vertList[2]];
    v3 = trunk->getMeshData()->vertexList[vertList[1]];
    v4 = trunk->getMeshData()->vertexList[vertList[3]];

    tree = branch_scale_rot_move(tree,v1,v2,v3,v4);
    n_branches++;

	return tree;
}

Tree* Tree::branch_scale_rot_move(Tree *t,Vert *v1, Vert *v2, Vert *v3,Vert *v4){
    Tree *b = t;
    b->branchLevel = branchLevel+1;
	int angle = prop->rotVals[branchLevel+1];
	float girth = prop->girths[branchLevel+1];
    //scale branch
	b->trunk->scaleSegment(0,b->trunk->getNLevels(),girth);
	b->trunk->scaleSegment(b->trunk->getNEdges()+1,(b->trunk->getNEdges()+1)*2,girth);
	
	b->upVector = Vert::getNormalVector(b->trunk->getMeshData()->vertexList[0],b->trunk->getMeshData()->vertexList[2],b->trunk->getMeshData()->vertexList[1]);

    //rotate branch
	Vert *normalVector = Vert::getNormalVector(v1,v2,v3);
	Vert *v = Vert::getRotationAxis(b->upVector,normalVector);
	double sAngle = Vert::getScalarAngle(b->upVector,normalVector);

    rotateBranch(b->trunk,v,static_cast<float>(sAngle));
	//rotateBranch(b->trunk,normalVector,static_cast<float>(PI/2));

	Vert *vRotated = Vert::getRotationAxis(normalVector,upVector);

	Vert *temp1 = new Vert(v1->x + normalVector->x, v1->y + normalVector->y, v1->z + normalVector->z);
	Vert *temp2 =  Vert::getNormalVector(temp1,v3,v1);
	std::vector<int> vertList1 = b->trunk->getVertexIndexFromPlane(1);
	Vert *temp3 = Vert::getNormalVector( b->trunk->getMeshData()->vertexList[vertList1[0]],b->trunk->getMeshData()->vertexList[vertList1[2]], b->trunk->getMeshData()->vertexList[vertList1[1]]);
	Vert *temp4 = Vert::getRotationAxis(temp2,temp3);
	sAngle = Vert::getScalarAngle(temp2,temp3);
	rotateBranch(b->trunk,normalVector,sAngle);

	float rads = (static_cast<float>(PI)*angle)/180;
    rotateBranch(b->trunk,vRotated,rads);

	// upvector
	b->upVector = Vert::getNormalVector(b->trunk->getMeshData()->vertexList[0],b->trunk->getMeshData()->vertexList[2],b->trunk->getMeshData()->vertexList[1]);
	

    //move branch
    Vert *p1 = trunk->getCenterPoint(v1,v2,v3,v4);
	std::vector<Vert*> vertList;
	for(int i = 0;i < b->trunk->getNEdges()+1;i++){
		vertList.push_back(b->trunk->getMeshData()->vertexList[i]);
		Vert *v = b->trunk->getMeshData()->vertexList[i];
	}
	Vert *p2 = b->trunk->getCenterPoint(vertList);
	b->trunk->getMeshData()->moveMesh(p1,p2);

	delete normalVector;
	delete v;
	delete vRotated;
	delete p1;
	delete p2;
	return b;
}


void Tree::rotateBranch(cylinder *c,Vert *v, float angle){
    cylinder *cyl = c;
	meshData *m = cyl->getMeshData();
    for(int i = 0;i < cyl->getMeshData()->getListSize();i++){
        Vert u = *cyl->getMeshData()->vertexList[i];
        Vert *temp = Quaternion::quaternionRotation(v,angle,&u);
        m->vertexList[i]->x = temp->x;
		m->vertexList[i]->y = temp->y;
        m->vertexList[i]->z = temp->z;
    }
	cyl->setMeshData(m);
}



Vert *Tree::getBranchRotation(Tree *tree){

	int angle = rand() % 10;
	float rad = (angle*static_cast<float>(PI))/180;

	Vert v1 = *tree->trunk->getMeshData()->vertexList[0];
	Vert v2 = *tree->trunk->getMeshData()->vertexList[1];
	Vert v3 = *tree->trunk->getMeshData()->vertexList[2];
	Vert v4 = *tree->trunk->getMeshData()->vertexList[3];

	Vert *normalVector = Vert::getNormalVector(&v1,&v2,&v3);

	Vert *m = tree->trunk->getCenterPoint(&v1,&v2,&v3,&v4);
	Vert *up = new Vert(0,1,0);
	Vert *rotationDir = Vert::getNormalVector(normalVector,m,tree->upVector);
	Vert *temp = Quaternion::quaternionRotation(rotationDir,rad,tree->upVector);

	return temp;
}

void Tree::createLeaf(){
	Leaf *leaf = new Leaf();
	leafs.push_back(leaf);
	int randPos = 0;
	std::vector<int> vertList = trunk->getVertexIndexFromPlane(randPos);

    Vert *v1 = trunk->getMeshData()->vertexList[vertList[0]];
    Vert *v2 = trunk->getMeshData()->vertexList[vertList[2]];
    Vert *v3 = trunk->getMeshData()->vertexList[vertList[1]];
    Vert *v4 = trunk->getMeshData()->vertexList[vertList[3]];

	Vert *up = new Vert(0,0,1);
	Vert *normalVector = Vert::getNormalVector(v1,v2,v3);
	Vert *v = Vert::getRotationAxis(up,normalVector);
	double sAngle = Vert::getScalarAngle(up,normalVector);

	for(int i = 0; i < leaf->mesh->getListSize(); i++){
		Vert *vert = leaf->mesh->vertexList[i];
		Vert *temp = Quaternion::quaternionRotation(v,static_cast<float>(sAngle),vert);

		Vert *vRotated = Vert::getRotationAxis(normalVector,new Vert(0,1,0));

		Vert *temp2 = Quaternion::quaternionRotation(vRotated,1.57f,temp);
		Vert *temp3 = Quaternion::quaternionRotation(new Vert(0,1,0),1.57f*2.0f,temp2);
		leaf->mesh->vertexList[i] = temp3;
	}
	

	Vert *p1 = trunk->getCenterPoint(v1,v2,v3,v4);
	std::vector<Vert*> list;
	list.push_back(leaf->mesh->vertexList[0]);
	list.push_back(leaf->mesh->vertexList[2]);
	Vert *p2 = trunk->getCenterPoint(list);
	leaf->mesh->moveMesh(p1,p2);

	delete up;
	delete normalVector;
	delete v;

}

bool Tree::DoNotExist(std::vector<int> list, int item){
	for(size_t i = 0; i < list.size(); i++){
		if(list[i] == item){
			return false;
		}
	}
	return true;
}

void Tree::init(){
	for(int i = 0; i < branches.size(); i++){
		Tree *t = branches[i];
		t->init();
	}
	for(size_t i = 0; i < leafs.size(); i++){
		Leaf *l = leafs[i];
	}

}

int Tree::getNumberBranches(){
	
	
	int temp1,temp2 = 1;
	for(int i = 0; i < 6; i++){
		temp1 = 1;
		for(int j = 0; j < i; j++){
			temp1 *= prop->nBranchesList[j];
		}
		temp2 += temp1;
	}
	return temp2;
}

int Tree::getTotalBranches(){
	return totalBranches;
}

void Tree::setHeightPosition(Tree *tree,int pos){
	int height = pos / tree->trunk->getNEdges();
	heightOnTree = (double)height /  (double)tree->trunk->getNLevels();
}


void Tree::clearTree(){
	for(size_t i = 0; i < branches.size();i++){
		branches[i]->clearTree();
		for(size_t j = 0; j < leafs.size();j++){
			delete leafs[j];
		}
		delete branches[i];
	}
	delete trunk;
	totalBranches = 0;
}