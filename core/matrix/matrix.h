#include "../utils/bool.h"

struct Matrix
{
    int rows;
    int cols;
    double** data;
};

typedef struct Matrix Matrix;

enum MatrixMultiply
{
    I_J_K,
    I_K_J,
    K_I_J,
    K_J_I,
    J_I_K,
    J_K_I
};

typedef enum MatrixMultiply MatrixMultiply;


Matrix ReadFromFile(const char* filename);

Matrix GenerateRandom(const int size);

Matrix* CopyFrom(const Matrix* matrix);

Matrix* Transpose(const Matrix* matrix);

void PrintMatrix(const Matrix* matrix);

void FreeMatrix(Matrix* matrix);

Matrix MultiplyBy(const Matrix* matrixA, const Matrix* matrixB, MatrixMultiply mode);

bool WriteToFile(const Matrix* matrix, const char* filename, const bool isAdding);