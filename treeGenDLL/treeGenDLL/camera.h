#pragma once
#include <math.h>
const double PI = 3.1415926;
class Camera
{
public:
	double mX,mY,mZ,distance,offX,offY,offZ,camAngleX,camAngleY;
	int SCREEN_HEIGHT, SCREEN_WIDTH,lMX,lMY;
	Camera(int SCREEN_HEIGHT,int SCREEN_WIDTH);
	void updateCamera();
	void mousePosRelease(int x, int y);
	void mousePosPress(int x, int y);
	void updateMousePos(int x, int y);
	double getXPos();
	double getYPos();
	double getZPos();
	void setDistance(double d);
	void updateOffset(int X, int Y);
	void mDown(int X, int Y);
	~Camera(void);
private:
	int tX,tY,lX,lY,lPX,lPY;


};




