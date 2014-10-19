#include "camera.h"
#include <iostream>
#include <stdio.h>
#include "Vert.h"
#include "quaternion.h"
#include "cylinder.h"
#include "tree.h"
#pragma unmanaged

Camera::Camera(int _SCREEN_HEIGHT, int _SCREEN_WIDTH)
{
	mX = 0.0;
	mY = 0.0;
	mZ = 1.0;

	offX = 0.0;
	offY = 0.0;
	offZ = 0.0;

	SCREEN_HEIGHT = _SCREEN_HEIGHT;
	SCREEN_WIDTH = _SCREEN_WIDTH;
	distance = 5, camAngleX = 1, camAngleY = 1;
}


Camera::~Camera(void)
{
}

void Camera::updateCamera () {
	
    int xPos = lPX;
    int yPos = (lPY*-1)+SCREEN_HEIGHT;

    camAngleX = (tX - xPos + lX)/(SCREEN_WIDTH/100);
    camAngleY = (tY - yPos + lY)/(SCREEN_HEIGHT/100);

    mX = 1 * -sin(camAngleX*(PI/180)) * cos((camAngleY)*(PI/180));
    mY = 1 * -sin((camAngleY)*(PI/180));
    mZ = -1 * cos((camAngleX)*(PI/180)) * cos((camAngleY)*(PI/180));
}

void Camera::mousePosRelease(int x, int y){
	lX += tX - x;
	lY += tY - (y*-1)+SCREEN_HEIGHT;
}

void Camera::mousePosPress(int x, int y){
	tX = x;
	tY = (y*-1)+SCREEN_HEIGHT;
}

void Camera::updateMousePos(int x, int y){
	lPX = x;
	lPY = y;
	updateCamera();
}

void Camera::setDistance(double d){
	double delta = (d/(100));
	if(distance + delta > 1){
		distance += delta;
	}
}

double Camera::getXPos(){
	return mX;
}
double Camera::getYPos(){
	return mY;
}
double Camera::getZPos(){
	return mZ;
}

void Camera::updateOffset(int X, int Y){

	Vert *n = new Vert(0,0,1);
	Vert *defPlane = new Vert(static_cast<float>((lMX-X)/100.0),static_cast<float>((Y-lMY)/100.0),static_cast<float>(0.0));

	Vert *rotationAxis;
	Vert *cVec = new Vert(static_cast<float>(mX),static_cast<float>(mY),static_cast<float>(mZ));
	rotationAxis = Vert::getRotationAxis(n,cVec);
	double sAngle = Vert::getScalarAngle(n,cVec);
	Vert *temp = Quaternion::quaternionRotation(rotationAxis,static_cast<float>(sAngle),defPlane);

	offX += temp->x;
	offY += temp->y;
	offZ += temp->z;
	lMY = Y;
	lMX = X;
}

void Camera::mDown(int X, int Y){
	lMX = X;
	lMY = Y;
}