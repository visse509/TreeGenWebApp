#include "quaternion.h"
#pragma unmanaged
Quaternion::Quaternion()
{
}
Quaternion* Quaternion::QuatFromAxisAngle(Vert *axis, float angleInRadians)
{
     Quaternion *result = new Quaternion();
     float angle = angleInRadians / 2.0f;
     float sinAngle = sin(angle);
     Vert *n = Vert::normalize(axis);


     result->x = n->x * sinAngle;
     result->y = n->y * sinAngle;
     result->z = n->z * sinAngle;
     result->w = cos(angle);

	 delete(n);

     return result;
}

Quaternion* Quaternion::quatMult(Quaternion *q1, Quaternion *q2){
    Quaternion *res = new Quaternion();
    float x1,x2,y1,y2,z1,z2,w1,w2;
    w1 = q1->w;
    w2 = q2->w;
    x1 = q1->x;
    x2 = q2->x;
    y1 = q1->y;
    y2 = q2->y;
    z1 = q1->z;
    z2 = q2->z;

    res->w = (w1*w2) - (x1*x2) - (y1*y2) - (z1*z2);
    res->x = (w1*x2) + (x1*w2) + (y1*z2) - (z1*y2);
    res->y = (w1*y2) - (x1*z2) + (y1*w2) + (z1*x2);
    res->z = (w1*z2) + (x1*y2) - (y1*x2) + (z1*w2);

    return res;
}

Quaternion* Quaternion::normalize(Quaternion *v){
    Quaternion *res = new Quaternion();
    float dist = sqrt(v->x*v->x + v->y*v->y + v->z*v->z +v->w*v->w);
    res->x = v->x/dist;
    res->y = v->y/dist;
    res->z = v->z/dist;
    res->w = v->w/dist;

    return res;
}

Vert* Quaternion::quaternionRotation(Vert *v, float angle, Vert *oldVector){
    Vert *res = new Vert();
	if(angle == 0){
		return oldVector;
	}
    Quaternion *q = Quaternion::QuatFromAxisAngle(v,angle);
    q = Quaternion::normalize(q);
    Quaternion *p =  new Quaternion();
    p->x = oldVector->x;
    p->y = oldVector->y;
    p->z = oldVector->z;
    p->w = 0;

    Quaternion *qInv = new Quaternion();
    qInv->x = -q->x;
    qInv->y = -q->y;
    qInv->z = -q->z;
    qInv->w =  q->w;

    Quaternion *temp = new Quaternion();
    temp = Quaternion::quatMult(q,p);

    Quaternion *temp2 = new Quaternion();
    temp2 = Quaternion::quatMult(temp,qInv);
    res->x = temp2->x;
    res->y = temp2->y;
    res->z = temp2->z;
    return res;
}

