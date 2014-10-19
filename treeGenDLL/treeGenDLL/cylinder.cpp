#include "cylinder.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#pragma unmanaged
#define PI 3.14159265

cylinder::cylinder(int edgeSize,float startLength,int maxSegments)
{
	m = new meshData(edgeSize*maxSegments*5, edgeSize*maxSegments*10);
    m->setNIndices(0);
    n_edges = edgeSize;
    n_levels = 0;
    cylinderHeightPos = 0;
    float revStep = 360/static_cast<float>(edgeSize);
	int i;
	float x, z;

    for(i = 0; i < edgeSize; i++){
        x = static_cast<float>(cos(PI*(i*revStep)/180.0f));
        z = static_cast<float>(sin(PI*(i*revStep)/180.0f));
		m->vertexList.push_back(new Vert(x, cylinderHeightPos, z));
    }
	x = 1;
    z = 0;
	m->vertexList.push_back(new Vert(x, cylinderHeightPos, z));

    for(i = 0; i < edgeSize; i++){
        x = static_cast<float>(cos(PI*(i*revStep)/180));
        z = static_cast<float>(sin(PI*(i*revStep)/180));
		m->vertexList.push_back( new Vert(x, cylinderHeightPos+startLength, z));
    }
	x = 1;
    z = 0;
  //  m->vertexList[i+edgeSize+1] = new Vert(x, cylinderHeightPos+startLength, z);
	m->vertexList.push_back( new Vert(x, cylinderHeightPos+startLength, z));

	m->setListSize(m->getListSize() + (edgeSize+1)*2);
    for( i = 0; i < edgeSize; i++){
        connectEdge(i, i+1, getIndexAtLevel(1, i), getIndexAtLevel(1, i+1));
    }
    n_levels += 1;
	setTexCoordInit();
    cylinderHeightPos += 2;

}

void cylinder::setTexCoordInit(){
	float width;
	for(int i = 0; i < (-n_edges+1); i++){
		width = (float)i/n_edges;
		m->texCords.push_back(width);
		m->texCords.push_back(0);
	}		

	for(int i = 0; i < (n_edges+1); i++){
		width = (float)i/n_edges;
		m->texCords.push_back(width);
		m->texCords.push_back(1.0f);
	}
}

void cylinder::setTexCoord(){
	float width;
	for(int i = 0; i < (n_edges+1); i++){
		width = (float)i/n_edges;
		m->texCords.push_back(width);
		m->texCords.push_back(static_cast<float>(n_levels));
	//	m->texCords.push_back(1.0f);
	}
	
}

void cylinder::connectEdge(int v1, int v2, int v3, int v4){
	m->index.push_back(v1);
    m->index.push_back(v3);
    m->index.push_back(v2);
    m->index.push_back(v2);
    m->index.push_back(v3);
    m->index.push_back(v4);
	m->setNIndices(m->getNIndices() + 6);

}

int cylinder::getIndexAtLevel(int level, int index){
    return ((n_edges+1)*level)+index;
}



Vert* cylinder::extrude(float scale,float length,Vert direction){

    for(int i = 0; i < n_edges+1; i++){
		m->vertexList.push_back(new Vert( m->vertexList[m->getListSize()+i-(n_edges+1)]));
    }
	m->setListSize(m->getListSize() + n_edges+1);

	Vert *normalVector = Vert::getNormalVector(m->vertexList[0],m->vertexList[2],m->vertexList[1]);
	normalVector = Vert::normalize(normalVector);
	scaleSegment(m->getListSize()-(n_edges+1),m->getListSize(), scale);

	double sAngle = Vert::getScalarAngle(normalVector,&direction);
    Vert *v = Vert::getRotationAxis(normalVector,&direction);
	if(v->isZero()){
		v->Set(direction);
	}

	Vert *temp = Quaternion::quaternionRotation(v,static_cast<float>(sAngle),normalVector);
	std::vector<Vert*> vertList;
	
	for(int i = 0; i < n_edges+1; i++){
		vertList.push_back(m->vertexList[m->getListSize()-(n_edges+1)+i]);
	}
	Vert *offVert = getCenterPoint(vertList);
    for(int i = 0; i < n_edges+1; i++){
		int offBack = m->getListSize()-(n_edges+1)+i;
        m->vertexList[offBack]->x += temp->x*length;
        m->vertexList[offBack]->y += temp->y*length;
        m->vertexList[offBack]->z += temp->z*length;

		m->vertexList[offBack]->x -= offVert->x;
		m->vertexList[offBack]->y -= offVert->y;
		m->vertexList[offBack]->z -= offVert->z;

		Vert *t = Quaternion::quaternionRotation(v,static_cast<float>(sAngle),m->vertexList[offBack]);
		m->vertexList[offBack]->x = t->x + offVert->x;
		m->vertexList[offBack]->y = t->y + offVert->y;
		m->vertexList[offBack]->z = t->z + offVert->z;

    }

    connectCap(n_levels);
    n_levels += 1;
    cylinderHeightPos += length;
	setTexCoord();

	delete(normalVector);
	delete(v);
	delete(offVert);

	return temp;
}


void cylinder::connectCap(int start){
    int i;
    int startPos = getIndexAtLevel(start,0);
    for(i = startPos; i < startPos+n_edges; i++){
        connectEdge(i, i+1, i+(n_edges+1), i+1+n_edges+1);
    }
}


void cylinder::scaleSegment(int start,int stop, float scaleValue){

    Vert *offSetVert = new Vert();
	std::vector<Vert*> vList;
    for(int i = start; i < start + n_edges + 1; i++){
		vList.push_back(m->vertexList[i]);
    }
	offSetVert = getCenterPoint(vList);
	for(int i = start; i < start + n_edges + 1; i++){

		m->vertexList[i]->x -= offSetVert->x;
		m->vertexList[i]->x *= scaleValue;
		m->vertexList[i]->x += offSetVert->x;

		m->vertexList[i]->y -= offSetVert->y;
		m->vertexList[i]->y *= scaleValue;
		m->vertexList[i]->y += offSetVert->y;

		m->vertexList[i]->z -= offSetVert->z;
		m->vertexList[i]->z *= scaleValue;
		m->vertexList[i]->z += offSetVert->z;
    }
	delete(offSetVert);
}



std::vector<int> cylinder::getVertexIndexFromPlane(int pos)
{
    std::vector<int> retList(4);

    retList[0] = pos;
    retList[2] = n_edges+1+pos;

    if((((pos+1) % (n_edges+1)) == 0) && (pos != 0))
    {
        retList[1] = pos-(n_edges);
        retList[3] = pos+1;
    }
    else
    {
        retList[1] = pos+1;
        retList[3] = pos+n_edges+2;
    }

    return retList;
}

void cylinder::rotateCylinder(cylinder *c, Vert *v, float angle){
    cylinder *cyl = c;

    for(int i = 0;i < cyl->m->getListSize();i++){
        Vert u = *cyl->m->vertexList[i];
        Vert *temp = Quaternion::quaternionRotation(v,angle,&u);
        cyl->m->vertexList[i]->x = temp->x;
        cyl->m->vertexList[i]->y = temp->y;
        cyl->m->vertexList[i]->z = temp->z;
    }

}


Vert* cylinder::getCenterPoint(Vert *v1, Vert *v2, Vert *v3, Vert *v4){

    float x,y,z;
    x = (v1->x+v2->x+v3->x+v4->x)/4;
    y = (v1->y+v2->y+v3->y+v4->y)/4;
    z = (v1->z+v2->z+v3->z+v4->z)/4;
    return new Vert(x,y,z);
}

Vert* cylinder::getCenterPoint(std::vector<Vert*> vertexList){

	if(vertexList.size() == 0){
		return new Vert(0,0,0);
	}

    float x = 0,y = 0,z = 0;
	for(size_t i = 0; i < vertexList.size(); i++){
		x += vertexList[i]->x;
		y += vertexList[i]->y;
		z += vertexList[i]->z;
	}
	return new Vert(x/vertexList.size(),y/vertexList.size(),z/vertexList.size());
}

meshData* cylinder::getMeshData(){
	return m;
}
void cylinder::setMeshData(meshData* _m){
	m = _m;
}
int cylinder::getNEdges(){
	return n_edges;
}
int cylinder::getNLevels(){
	return n_levels;
}
cylinder::~cylinder(){
	delete m;
}


