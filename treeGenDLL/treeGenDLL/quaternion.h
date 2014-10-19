
#ifndef QUATERNION_H
#define QUATERNION_H
#include "vert.h"
#include <math.h>

class Quaternion : public Vert
{
public:
    Quaternion();
    static Quaternion* QuatFromAxisAngle(Vert *axis, float angleInRadians);
    static Quaternion* quatMult(Quaternion *q1, Quaternion *q2);
    static Quaternion* normalize(Quaternion *v);
	static Vert* quaternionRotation(Vert *v, float angle, Vert *oldVector);
    float w;
};

#endif // QUATERNION_H
