#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"

struct matrix {
    int M;
    int N;
    float *data;
};

static void terminate (const char *message);
static int to1d(int i, int rowSize, int j);

Matrix MatrixCreate(int sizeX, int sizeY, float data[sizeX][sizeY]) {
    Matrix mat = malloc(sizeof(*mat));
    if (mat == NULL) {
        terminate("Error in MatrixCreate: data could not be created");        
    }
    mat->M = sizeX;
    mat->N = sizeY;
    mat->data = malloc(sizeof(float) * sizeX * sizeY);
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            (mat->data)[to1d(i, sizeY, j)] = data[i][j];
        }
    }

    return mat;
}

void MatrixDestroy(Matrix mat) {
    free(mat->data);
    free(mat);
}

Matrix MatrixTimes(Matrix A, Matrix B) {
    if (A->N != B->M) {
        MatrixPrint(A);
        printf("\n");
        MatrixPrint(B);
        terminate("Illegal matrix dimensions.");
    }

    float init[A->M][B->N];
    for (int i = 0; i < A->M; i++) {
        for (int j = 0; j < B->N; j++) {
            init[i][j] = 0.0f;
        }
    }
    Matrix C = MatrixCreate(A->M, B->N, init);

    for (int i = 0; i < C->M; i++) {
        for (int j = 0; j < C->N; j++) {
            for (int k = 0; k < A->N; k++) {
                (C->data)[to1d(i, C->N, j)] += (A->data)[to1d(i, A->N, k)] *
                        (B->data)[to1d(k, B->N, j)];
            }
        }
    }

    return C;
}

float MatrixGet(Matrix mat, int i, int j) {
    return mat->data[to1d(i, mat->N, j)];
}

void MatrixPrint(Matrix a) {
    for (int i = 0; i < a->M; i++) {
        for (int j = 0; j < a->N; j++) {
            printf("%f ", (a->data)[to1d(i, a->N, j)]);
        }
        printf("\n");
    }
}

static int to1d(int i, int rowSize, int j) {
    return i * rowSize + j;
}

static void terminate (const char *message) {
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}
