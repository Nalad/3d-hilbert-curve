#ifndef VECTOR3D
#define VECTOR3D

#include "Matrix.h"

typedef struct vector3d *Vector3D;

Vector3D Vec3DCreate(float x, float y, float z);
void Vec3DDestroy(Vector3D vec3d);
float Vec3DGetX(Vector3D vec3d);
float Vec3DGetY(Vector3D vec3d);
float Vec3DGetZ(Vector3D vec3d);
void Vec3DPrint(Vector3D vec3d);
Vector3D Vec3DTo2D(Vector3D vec3d, Matrix mvp, int pictureWidth);
Vector3D Vec3DCross(Vector3D a, Vector3D b);
float Vec3DLength(Vector3D a);
void Vec3DNormalize(Vector3D a);
float Vec3DDot(Vector3D a, Vector3D b);

#endif
