#include "meshdata.h"
#include "TexCord.h"
#pragma managed
const double PI = 3.1415926;
meshData::meshData(int vertexSize, int indiceSize)
{
	listSize = 0;
}

float* meshData::getData(){
    float* data;
    data = new float[listSize*3];

    for(int i = 0; i < listSize*3; i++)
        data[i] = 0.0;


    for(int i = 0; i < listSize;i++){
        data[i*3] = vertexList[i]->x;
        data[i*3+1] = vertexList[i]->y;
        data[i*3+2] = vertexList[i]->z;
    }
    return data;
}


void meshData::setNIndices(int n){
	n_indices = n;
}

int meshData::getNIndices(){
	return n_indices;
}

void meshData::setListSize(int size){
	listSize = size;
}

int meshData::getListSize(){
	return listSize;
}
void meshData::move(float x, float y, float z){
    for(int i = 0; i < listSize; i++){
        float newX = vertexList[i]->x + x;
        float newY = vertexList[i]->y + y;
        float newZ = vertexList[i]->z + z;
        vertexList[i]->Set(Vert(newX, newY, newZ));

    }
}

void meshData::moveMesh(Vert *p1, Vert *p2){
    for(int i  = 0; i < listSize;i++){
		vertexList[i]->x += (p1->x - p2->x);
        vertexList[i]->y += (p1->y - p2->y);
        vertexList[i]->z += (p1->z - p2->z);
    }
}
meshData::~meshData(){

}
