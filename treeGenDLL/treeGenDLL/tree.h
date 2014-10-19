
#ifndef TREE_H
#define TREE_H
#include "cylinder.h"
#include <vector>
#include "quaternion.h"
#include "TreeProperty.h"
#include "Leaf.h"
#include <random>
#include <iostream>
#include "stringGenerator.h"

class Tree{
private:
	std::vector<char> parseString();
	std::vector<char> charList;
public:
    Tree(int nEdges,int _branchLevel,std::string propXml,std::string lsXml);
	Tree(int nEdges,int _branchLevel);
    int treeLevel;
	cylinder *trunk;
    std::vector<Tree*> branches;
	std::vector<Leaf*> leafs;
    void draw();
    void createTree();
    void init();
    float branchChance;
    int random;
    int n_branches;
    Vert *upVector;
	TreeProperty *prop;
    int branchLevel;
	double heightOnTree;
    void     createBranchRecursive( int branchLevels);
    Tree* branch_scale_rot_move(Tree *t, Vert *v1, Vert *v2,Vert *v3, Vert *v4);
    void rotateBranch(cylinder *c,Vert *v,float angle);
    void rotateVert(Vert *v,float xAngle, float yAngle, float zAngle);
    float getScalarAngle(Vert *v1,Vert *v2);
    Tree* init_branch(int nEdges, int pos);
    void drawSelections();
	Vert *getBranchRotation(Tree* tree);
    int getRandPos();
	std::vector<int> branchPos;
	int getNumberBranches();
	bool DoNotExist(std::vector<int> list, int item);
	void createBranch(int branchLevel);
	void createLeaf();
	int getTotalBranches();
	void extrudeTrunk();
	void setHeightPosition(Tree *tree,int pos);
	double getLengthFromHeightPosOnTree(double height);
	void clearTree();
	std::default_random_engine generator;
	std::normal_distribution<double> distribution;
	void Tree::createTreeLS();
};

#endif // TREE_H
