#include "Leaf.h"


Leaf::Leaf(void)
{
	mesh = new meshData(4, 6);
	createTexCoord();
	createLeafMesh();
	//mesh->initMesh();
	//mesh->move(5.0,0,5.0);
	
}

void Leaf::createLeafMesh(){
	mesh->vertexList.push_back(new Vert(-0.2f,0.0f,-0.2f));
	mesh->vertexList.push_back(new Vert(-0.2f,0.4f,-0.2f));
	mesh->vertexList.push_back(new Vert(0.2f,0.0f,-0.2f));
	mesh->vertexList.push_back(new Vert(0.2f,0.5f,-0.2f));

	mesh->index.push_back(0);
	mesh->index.push_back(2);
	mesh->index.push_back(1);
	mesh->index.push_back(1);
	mesh->index.push_back(2);
	mesh->index.push_back(3);

	mesh->setNIndices(6);
	mesh->setListSize(4);

}

void Leaf::createTexCoord(){
	mesh->texCords.push_back(0);
	mesh->texCords.push_back(0);

	mesh->texCords.push_back(0);
	mesh->texCords.push_back(1);

	mesh->texCords.push_back(1);
	mesh->texCords.push_back(0);

	mesh->texCords.push_back(1);
	mesh->texCords.push_back(1);
}

Leaf::~Leaf(void)
{
	delete mesh;
}