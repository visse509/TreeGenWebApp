#pragma once
#include "meshdata.h"

class Leaf
{
public:
	Leaf(void);
	meshData *mesh;
	~Leaf(void);
	void createLeafMesh();
	void createTexCoord();
	void draw();
};

