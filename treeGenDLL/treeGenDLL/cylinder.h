
#ifndef CYLINDER_H
#define CYLINDER_H
#include "meshdata.h"
#include "quaternion.h"

class cylinder
{
public:
    cylinder(int size,float startLength,int maxSegments);
	~cylinder();

    Vert* extrude(float scale,float length,Vert direction);
    void mergeCylinder(cylinder *c1);
	void setTexCoord();

	void rotateCylinder(cylinder *c,Vert *v, float angle);
	Vert* getCenterPoint(Vert *v1, Vert *v2, Vert *v3, Vert *v4);
	Vert* getCenterPoint(std::vector<Vert*> vertexList);
    
	int getNEdges();
	int getNLevels();

	std::vector<int> getVertexIndexFromPlane(int pos);
	void scaleSegment(int start,int stop, float scaleValue);

	void setMeshData(meshData* _m);
	meshData* getMeshData();

private:

	meshData *m;
    int n_edges;
    int n_levels;
	float cylinderHeightPos;
    
	void setTexCoordInit();
	void deleteLevel();

	void connectCap(int start);
	void connectEdge(int v1, int v2, int v3, int v4);
	int getIndexAtLevel(int level, int index);
    
};


#endif // CYLINDER_H
