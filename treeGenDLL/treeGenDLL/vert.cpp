#include "vert.h"
#include <math.h>
#pragma unmanaged

Vert::Vert(float _x,float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Vert::Vert(Vert *v)
{
    x = v->x;
	y = v->y;
    z = v->z;
}

Vert::Vert(){
	x = 0;
	y = 0;
	z = 0;
}
void Vert::Set(Vert v){
       x = v.x;
       y = v.y;
       z = v.z;
}

Vert* Vert::normalize(Vert *v){
    Vert *res = new Vert();
    double dist = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    res->x = v->x/dist;
    res->y = v->y/dist;
    res->z = v->z/dist;


    return res;
}

bool Vert::isZero(){
    if(x == 0){
        if(y == 0){
            if(z == 0){
                return true;
            }
        }
    }
	return false;
}

bool Vert::isEqual(Vert *v){
	if(x == v->x){
		if(y == v->y){
			if(z == v->z){
				return true;
			}
		}
	}
	return false;
}

Vert* Vert::getNormalVector(Vert *v1, Vert *v2, Vert *v3){
    Vert *temp1 = new Vert();
    Vert *temp2 = new Vert();
    Vert *res = new Vert();

    temp1->x = (v2->x - v1->x);
    temp1->y = (v2->y - v1->y);
    temp1->z = (v2->z - v1->z);

    temp2->x = (v3->x - v1->x);
    temp2->y = (v3->y - v1->y);
    temp2->z = (v3->z - v1->z);


    res->x = temp1->y*temp2->z - temp1->z*temp2->y;
    res->y = temp1->z*temp2->x - temp1->x*temp2->z;
    res->z = temp1->x*temp2->y - temp1->y*temp2->x;


    float dist = sqrt(res->x*res->x + res->y*res->y + res->z*res->z);

	res->x = res->x/dist;
	res->y = res->y/dist;
	res->z = res->z/dist;


	delete(temp1);
	delete(temp2);
	
    return res;
}

double Vert::getScalarAngle(Vert *v1,Vert *v2){

    float ls = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);
    float d1 = sqrt((v1->x*v1->x) + (v1->y*v1->y) + (v1->z*v1->z));
    float d2 = sqrt((v2->x*v2->x) + (v2->y*v2->y) + (v2->z*v2->z));
	if(abs(ls-1) < 0.0001){
		return 0;
	}
    return acos(ls);
}

Vert* Vert::getRotationAxis(Vert *v1, Vert *v2){
    Vert *temp1 = new Vert();
    Vert *temp2 = new Vert();
    Vert *res = new Vert();
    Vert *res2 = new Vert();

    temp1->x  = (v1->y*v2->z)-(v2->y*v1->z);
    temp1->y  = -(v1->x*v2->z)+(v2->x*v1->z);
    temp1->z  = (v1->x*v2->y)-(v1->y*v2->x);

	delete(temp2);
	delete(res);
	delete(res2);

    return temp1;
}
std::string Vert::VertToString(Vert *v){
	std::ostringstream convert;
	std::string res = "[";
	std::string val = static_cast<std::ostringstream*>( &(std::ostringstream() << v->x) )->str();
	res.append(val);
	res.append("\t");
	val = static_cast<std::ostringstream*>( &(std::ostringstream() << v->y) )->str();
	res.append(val);
	res.append("\t");
	val = static_cast<std::ostringstream*>( &(std::ostringstream() << v->z) )->str();
	res.append(val);
	res.append("]");
	if(val == "-1.#IND"){
		int f = 5;
	};

	return res;
}

Vert::~Vert(){

}