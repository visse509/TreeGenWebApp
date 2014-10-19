#ifndef MESHDATA_H
#define MESHDATA_H
#include "vert.h"
#include <vector>
#include <math.h>


class meshData
{

public:
    meshData(int vertexSize, int indiceSize);
	~meshData();

	void draw();
    void addVboData();
    void initMesh();
	void clearVBO();

	void setNIndices(int n);
	int getNIndices();

	void setListSize(int size);
	int getListSize();

	void move(float x, float y, float z);
	void moveMesh(Vert *p1, Vert *p2);

	std::vector<Vert*> vertexList;
	std::vector<float> texCords;
	std::vector<unsigned short> index;
	//unsigned short *index;

private:
	int listSize;
    int n_indices;

    unsigned int indiceId;
    unsigned int vertexId;
	unsigned int textureCoordId;
	float* getData();
};


#endif // MESHDATA_H
