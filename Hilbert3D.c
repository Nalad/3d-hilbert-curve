#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "List.h"
#include "Matrix.h"
#include "Vector3D.h"

#define PI 3.141592

void draw(int depth, int pictureWidth, float cubeWidth, float distance,
        float xOrigin, float yOrigin, float zOrigin, float xAngle,
        float yAngle);
List hilbert3D (Vector3D center, float width, int depth,
        int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7);
Matrix modelMatrix(float x, float y, float z, float xAngle, float yAngle);
Matrix viewMatrix(Vector3D observer, Vector3D direction, Vector3D up);
Matrix projectionMatrix(float fov, float aspect, float nearDist, float farDist);

int main(int argc, char *argv[]) {
    if (argc != 10) {
        fprintf(stderr, "Bad amount of arguments\n");
        exit(EXIT_FAILURE);
    }

    int depth;
    sscanf(argv[1], "%d", &depth);
    int pictureWidth;
    sscanf(argv[2], "%d", &pictureWidth);
    float cubeWidth;
    sscanf(argv[3], "%f", &cubeWidth);
    float distance;
    sscanf(argv[4], "%f", &distance);
    float xOrigin, yOrigin, zOrigin;
    sscanf(argv[5], "%f", &xOrigin);
    sscanf(argv[6], "%f", &yOrigin);
    sscanf(argv[7], "%f", &zOrigin);
    float xAngle, yAngle;
    sscanf(argv[8], "%f", &xAngle);
    sscanf(argv[9], "%f", &yAngle);

    if (depth >= 1) {
        draw(depth, pictureWidth, cubeWidth, distance, xOrigin, yOrigin, zOrigin, xAngle, yAngle);
    }
 
    return 0;
}


void draw(int depth, int pictureWidth, float cubeWidth, float distance, float xOrigin, float yOrigin, float zOrigin, float xAngle, float yAngle) {
    List vecs = hilbert3D(Vec3DCreate(0.0f, 0.0f, 0.0f), cubeWidth, depth, 0, 1, 2, 3, 4, 5, 6, 7);

    // Add minus to xAngle or yAngle if we want opposite rotation
    Matrix model = modelMatrix(xOrigin, yOrigin, zOrigin, xAngle, yAngle);

    Vector3D observer = Vec3DCreate(0.0f, 0.0f, distance);
    Vector3D direction = Vec3DCreate(0.0f, 0.0f, 0.0f);
    Vector3D up = Vec3DCreate(0.0f, 1.0f, 0.0f);
    Matrix view = viewMatrix(observer, direction, up);
    Vec3DDestroy(up);
    Vec3DDestroy(direction);
    Vec3DDestroy(observer);

    Matrix projection = projectionMatrix(90.0f, 1.0f, 0.1f, 100.0f);

    Matrix vp = MatrixTimes(projection, view);
    Matrix mvp = MatrixTimes(vp, model);
    MatrixDestroy(vp);
    MatrixDestroy(projection);
    MatrixDestroy(view);
    MatrixDestroy(model);

    FILE *file = fopen("./picture.eps", "w");
    fprintf(file, 
            "%%%%!PS-Adobe-2.0 EPSF-2.0\n%%BoundingBox: 0 0 %d %d\nnewpath\n",
            pictureWidth, pictureWidth);


    Vector3D point = ListRemoveLastItem(vecs);
    Vector3D windowCoordinates = Vec3DTo2D(point, mvp, pictureWidth);
    Vec3DDestroy(point);
    fprintf(file, "%.2f %.2f moveto\n", Vec3DGetX(windowCoordinates), Vec3DGetY(windowCoordinates));
    Vec3DDestroy(windowCoordinates);
    while (!ListIsEmpty(vecs)) {
        point = ListRemoveLastItem(vecs);
        windowCoordinates = Vec3DTo2D(point, mvp, pictureWidth);
        Vec3DDestroy(point);
        fprintf(file, "%.2f %.2f lineto\n",
                Vec3DGetX(windowCoordinates),
                Vec3DGetY(windowCoordinates));
        Vec3DDestroy(windowCoordinates);

    }
    fprintf(file, ".4 setlinewidth\nstroke\nshowpage\n%%%%Trailer\n%%EOF");

    fclose(file);
    MatrixDestroy(mvp);
    ListDestroy(vecs);
}

Matrix modelMatrix(float x, float y, float z, float xAngle, float yAngle) {
     float translationValues[4][4] = {
            {1.0f,   0.0f,    0.0f,    x},
            {0.0f,   1.0f,    0.0f,    y},
            {0.0f,   0.0f,    1.0f,    z},
            {0.0f,   0.0f,    0.0f,    1.0f}
    };
    Matrix translation = MatrixCreate(4, 4, translationValues);

    float rotationXValues[4][4] = {
            {1.0f,   0.0f,                          0.0f,                           0.0f},
            {0.0f,   cos(xAngle * (PI / 180.0f)),   -sin(xAngle * (PI / 180.0f)),   0.0f},
            {0.0f,   sin(xAngle * (PI / 180.0f)),   cos(xAngle * (PI / 180.0f)),    0.0f},
            {0.0f,   0.0f,                          0.0f,                           1.0f}
    };
    Matrix rotationX = MatrixCreate(4, 4, rotationXValues);

    float rotationYValues[4][4] = {
            {cos(yAngle * (PI / 180.0f)),   0.0f,    sin(yAngle * (PI / 180.0f)),   0.0f},
            {0.0f,                          1.0f,    0.0f,                          0.0f},
            {-sin(yAngle * (PI / 180.0f)),  0.0f,    cos(yAngle * (PI / 180.0f)),   0.0f},
            {0.0f,                          0.0f,    0.0f,                          1.0f}
    };
    Matrix rotationY = MatrixCreate(4, 4, rotationYValues);
    Matrix rotation = MatrixTimes(rotationX, rotationY);

    Matrix model = MatrixTimes(translation, rotation);
    MatrixDestroy(rotation);
    MatrixDestroy(rotationY);
    MatrixDestroy(rotationX);
    MatrixDestroy(translation);

    return model;
}

Matrix viewMatrix(Vector3D observer, Vector3D direction, Vector3D up) {
    Vector3D vector = Vec3DCreate(
            Vec3DGetX(direction) - Vec3DGetX(observer),
            Vec3DGetY(direction) - Vec3DGetY(observer),
            Vec3DGetZ(direction) - Vec3DGetZ(observer));
    Vec3DNormalize(vector);

    Vector3D rV = Vec3DCross(up, vector);
    Vector3D uV = Vec3DCross(vector, rV);

    Vec3DNormalize(rV);
    Vec3DNormalize(uV);

    float out[4][4];
    
    out[0][0] = Vec3DGetX(rV);
    out[0][1] = Vec3DGetY(rV);
    out[0][2] = Vec3DGetZ(rV);
    out[0][3] = -Vec3DDot(rV, observer);
    out[1][0] = Vec3DGetX(uV);
    out[1][1] = Vec3DGetY(uV);
    out[1][2] = Vec3DGetZ(uV);
    out[1][3] = -Vec3DDot(uV, observer);
    out[2][0] = Vec3DGetX(vector);
    out[2][1] = Vec3DGetY(vector);
    out[2][2] = Vec3DGetZ(vector);
    out[2][3] = -Vec3DDot(vector, observer);
    out[3][0] = 0.0f;
    out[3][1] = 0.0f;
    out[3][2] = 0.0f;
    out[3][3] = 1.0f;
     
    Vec3DDestroy(uV);
    Vec3DDestroy(rV);
    Vec3DDestroy(vector);

    return MatrixCreate(4, 4, out);
}

Matrix projectionMatrix(float angleOfView, float aspect, float nearDist, float farDist) {
    
    float out[4][4];

    out[0][0] = 1.0f / (aspect * tan(angleOfView * 0.5 * PI / 180));
    out[0][1] = 0.0f;
    out[0][2] = 0.0f;
    out[0][3] = 0.0f;
    out[1][0] = 0.0f;
    out[1][1] = 1.0f / tan(angleOfView * 0.5 * PI / 180);
    out[1][2] = 0.0f;
    out[1][3] = 0.0f;
    out[2][0] = 0.0f;
    out[2][1] = 0.0f;
    out[2][2] = -(farDist + nearDist) / (farDist - nearDist);
    out[2][3] = -2.0f * (nearDist * farDist) / (farDist - nearDist);
    out[3][0] = 0.0f;
    out[3][1] = 0.0f;
    out[3][2] = -1.0f;
    out[3][3] = 0.0f;

    return MatrixCreate(4, 4, out);
}

List hilbert3D (Vector3D center, float width, int depth,
        int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7) {
    int centerX = Vec3DGetX(center);
    int centerY = Vec3DGetY(center);
    int centerZ = Vec3DGetZ(center);
    Vec3DDestroy(center);

    Vector3D vecs[] = {
        Vec3DCreate(centerX - width/2, centerY + width/2, centerZ - width/2),
        Vec3DCreate(centerX - width/2, centerY + width/2, centerZ + width/2),
        Vec3DCreate(centerX - width/2, centerY - width/2, centerZ + width/2),
        Vec3DCreate(centerX - width/2, centerY - width/2, centerZ - width/2),
        Vec3DCreate(centerX + width/2, centerY - width/2, centerZ - width/2),
        Vec3DCreate(centerX + width/2, centerY - width/2, centerZ + width/2),
        Vec3DCreate(centerX + width/2, centerY + width/2, centerZ + width/2),
        Vec3DCreate(centerX + width/2, centerY + width/2, centerZ - width/2),
    };

    List gray = ListCreate();
    ListAdd(gray, vecs[v7]);
    ListAdd(gray, vecs[v6]);
    ListAdd(gray, vecs[v5]);
    ListAdd(gray, vecs[v4]);
    ListAdd(gray, vecs[v3]);
    ListAdd(gray, vecs[v2]);
    ListAdd(gray, vecs[v1]);
    ListAdd(gray, vecs[v0]);

    if (depth <= 1) return gray;

    List verts = ListCreate();
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v0, v3, v4, v7, v6, v5, v2, v1));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v0, v7, v6, v1, v2, v5, v4, v3));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v0, v7, v6, v1, v2, v5, v4, v3));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v2, v3, v0, v1, v6, v7, v4, v5));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v2, v3, v0, v1, v6, v7, v4, v5));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v4, v3, v2, v5, v6, v1, v0, v7));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v4, v3, v2, v5, v6, v1, v0, v7));
    ListAddAll(verts, hilbert3D(ListRemoveLastItem(gray), width/2, depth - 1, v6, v5, v2, v1, v0, v3, v4, v7));
    ListDestroy(gray);
    
    return verts;
}
