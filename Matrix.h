#ifndef MATRIX
#define MATRIX

typedef struct matrix *Matrix;

Matrix MatrixCreate(int sizeX, int sizeY, float data[sizeX][sizeY]);
void MatrixDestroy(Matrix mat);
float MatrixGet(Matrix mat, int i, int j);
void MatrixPrint(Matrix a);
Matrix MatrixTimes(Matrix A, Matrix B);

#endif
