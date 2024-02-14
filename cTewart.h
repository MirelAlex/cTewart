#ifndef CTEWART_H
#define CTEWART_H

#include "types.h"

typedef struct{
    Vector3 pos;
    float radius;
    float shaftDistance;
} Base;

typedef struct {
    Vector3 pos;
    float radius;
    float anchorDistance;
} Platform;

typedef struct
{
    Vector3 baseJoint;
    Vector3 platformJoint;
    float motorRot;
} Legs;

typedef struct {
    Vector3 pos;
    Vector3 translation;
    Quaternion orientation;
    float legsLength;
    float hornLength;
    float rodLength;
    Legs legs[6];
    Vector3 B[6];
    Vector3 P[6];
    Vector3 q[6];
    Vector3 l[6];
    Vector3 H[6];
    Vector3 T0;
    float sinBeta[6];
    float cosBeta[6];
    Base base;
    Platform plat;
} cTewart;

cTewart stewartPlatform;

#define THIS stewartPlatform
#define BASE (THIS.base)
#define PLAT (THIS.plat)
#define zero_vec (Vector3){0.0f, 0.0f, 0.0f}
#define zero_qua (Quaternion){0.0f, 0.0f, 0.0f, 0.0f}

#endif // CTEWART_H
