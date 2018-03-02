#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"
#include "Vector3D.h"

struct vector3d {
    float x, y, z;
};

static void terminate (const char *message);

Vector3D Vec3DCreate(float x, float y, float z) {
    Vector3D vec3d = malloc(sizeof(*vec3d));
    if (vec3d == NULL) {
        terminate("Error in createVec3D: vec3d could not be created.");
    }
    vec3d->x = x;
    vec3d->y = y;
    vec3d->z = z;

    return vec3d;
}

void Vec3DDestroy(Vector3D vec3d) {
    free(vec3d);
}

float Vec3DGetX(Vector3D vec3d) {
    return vec3d->x;
}

float Vec3DGetY(Vector3D vec3d) {
    return vec3d->y;
}

float Vec3DGetZ(Vector3D vec3d) {
    return vec3d->z;
}

void Vec3DPrint(Vector3D vec3d) {
    printf("(%.6f, %.6f, %.6f)", vec3d->x, vec3d->y, vec3d->z);
}

Vector3D Vec3DTo2D(Vector3D point, Matrix mvp, int pictureWidth) {
    float coordinatesValues[4][1] = {
        {point->x},
        {point->y},
        {point->z},
        {1.0}
    };

    Matrix coordinates = MatrixCreate(4, 1, coordinatesValues);
    Matrix window = MatrixTimes(mvp, coordinates);
    MatrixDestroy(coordinates);

    float x3d = MatrixGet(window, 0, 0);
    float y3d = MatrixGet(window, 1, 0);
    float w3d = MatrixGet(window, 3, 0);
    MatrixDestroy(window);
    Vector3D windowCoordinates = Vec3DCreate(
            pictureWidth * 0.5f * x3d / w3d + x3d + pictureWidth * 0.5f,
            pictureWidth * 0.5f * y3d / w3d + y3d + pictureWidth * 0.5f,
            0.0f);

    return windowCoordinates;
}

Vector3D Vec3DCross(Vector3D a, Vector3D b) {
    Vector3D cross = Vec3DCreate(
            a->y * b->z - a->z * b->y,
            a->z * b->x - a->x * b->z,
            a->x * b->y - a->y * b->x);
    return cross;
}

float Vec3DLength(Vector3D a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

void Vec3DNormalize(Vector3D a) {
    float length = Vec3DLength(a);
    a->x /= length;
    a->y /= length;
    a->z /= length;
}

float Vec3DDot(Vector3D a, Vector3D b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static void terminate (const char *message) {
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}
