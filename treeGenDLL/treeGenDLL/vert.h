#ifndef VERT_H
#define VERT_H
#include <iostream>
#include <fstream>
#include <sstream> 
class Vert
{
    public:
        Vert(float _x,float _y, float _z);
		~Vert();

        float x,y,z;
        void Set(Vert v);
		Vert(Vert *v);
		
        static Vert* normalize(Vert *v);
		bool isEqual(Vert *v);
		static Vert* getNormalVector(Vert *v1, Vert *v2, Vert *v3);
		static double getScalarAngle(Vert *v1,Vert *v2);
		static Vert* getRotationAxis(Vert *v1, Vert *v2);
		static std::string VertToString(Vert *v);
        bool isZero();

		Vert(); 
	private:

};


#endif // VERT_H
