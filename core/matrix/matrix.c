#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "../utils/bool.h"
#include "../utils/ioUtils.h"
#include "../utils/math.h"

const int MAX_ELEMENTS_TO_PRINT = 100;

const double RANDOM_MATRIX_MIN = 0;
const double RANDOM_MATRIX_MAX = 100;


#pragma region private methods prototypes 
double** ReadMatrixFromFile(const char* filename, int* rows, int* cols);
#pragma endregion


#pragma region public methods

Matrix ReadFromFile(const char* filename)
{
    int rows;
    int cols;
    double** data = ReadMatrixFromFile(filename, &rows, &cols);

    if (data == NULL)
    {
        Matrix empty = {0, 0, NULL};
        return empty;
    }

    Matrix matrix = {rows, cols, data};
    return matrix;
}


Matrix GenerateRandom(const int size)
{
    Matrix random = {size, size, NULL};
    
    if (size <= 0)
    {
        printf("Размер матрицы должен быть положительным\n");
        return random;
    }
    
    random.data = (double**)malloc(size * sizeof(double*));
    if (random.data == NULL)
    {
        printf("Ошибка выделения памяти для строк\n");
        random.rows = 0;
        random.cols = 0;
        return random;
    }
    

    for (int i = 0; i < size; i++)
    {
        random.data[i] = (double*)malloc(size * sizeof(double));
        
        if (random.data[i] == NULL) 
        {
            printf("Ошибка выделения памяти для строки %d\n", i);

            for (int k = 0; k < i; k++)
                free(random.data[k]);
            free(random.data);
            random.data = NULL;
            random.rows = 0;
            random.cols = 0;
            return random;
        }


        for (int j = 0; j < size; j++)
        {
            random.data[i][j] = RandomDouble(RANDOM_MATRIX_MIN, RANDOM_MATRIX_MAX);
        }
    }
    
    return random;
}


Matrix* CopyFrom(const Matrix* matrix)
{
    if (matrix == NULL || matrix->data == NULL)
    {
        printf("Ошибка: исходная матрица пуста\n");
        return NULL;
    }
    
    Matrix* new = (Matrix*)malloc(sizeof(Matrix));
    if (new == NULL)
    {
        return NULL;
    }

    new->rows = matrix->rows;
    new->cols = matrix->cols;
    

    new->data = (double**)malloc(new->rows * sizeof(double*));
    if (new->data == NULL)
    {
        free(new);
        return NULL;
    }

    for (int i = 0; i < new->rows; i++)
    {
        new->data[i] = (double*)malloc(new->cols * sizeof(double));
        if (new->data[i] == NULL)
        {
            for (int k = 0; k < i; k++)
                free(new->data[k]);
            free(new->data);
            free(new); 
            return NULL;
        }
        
        for (int j = 0; j < new->cols; j++)
        {
            new->data[i][j] = matrix->data[i][j];
        }
    }
    
    return new;
}

Matrix* Transpose(const Matrix* matrix)
{
    if (matrix == NULL || matrix->data == NULL)
    {
        printf("Ошибка: исходная матрица пуста\n");
        return NULL;
    }
    
    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    if (result == NULL)
    {
        free(result);
        return NULL;
    }

    result->rows = matrix->cols;
    result->cols = matrix->rows;
    
    result->data = (double**)malloc(result->rows * sizeof(double*));
    if (result->data == NULL)
    {
        free(result);
        return NULL;
    }

    for (int i = 0; i < result->rows; i++)
    {
        result->data[i] = (double*)malloc(result->cols * sizeof(double));
        if (result->data[i] == NULL)
        {
            for (int k = 0; k < i; k++)
                free(result->data[k]);
            free(result->data);
            free(result); 
            return NULL;
        }
    }
    
    // Transpose: result[j][i] = matrix[i][j]
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            result->data[j][i] = matrix->data[i][j];
        }
    }
    
    return result;
}

void PrintMatrix(const Matrix* matrix)
{

    if (matrix == NULL || matrix->data == NULL)
    {
        printf("Matrix is empty\n");
        return;
    }
    
    printf("Матрица %d x %d\n", matrix->rows, matrix->cols);
    if (matrix->rows * matrix-> cols > MAX_ELEMENTS_TO_PRINT) return;
    
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            printf("%8.2f ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void FreeMatrix(Matrix* matrix)
{
    if (matrix == NULL) return;

    if (matrix->data != NULL)
    {
        for (int i = 0; i < matrix->rows; i++)
        {
            if (matrix->data[i] != NULL)
                free(matrix->data[i]);
        }
        free(matrix->data);
    }
    
    matrix->rows = 0;
    matrix->cols = 0;
    matrix->data = NULL;
}

Matrix MultiplyBy(const Matrix* matrixA, const Matrix* matrixB, MatrixMultiply mode)
{

    Matrix c = {0, 0, NULL};

    if (matrixA == NULL || matrixB == NULL)
    {
        printf("matrix a and b must be not null!\n");
        return c;
    }


    c.rows = matrixA->rows;
    c.cols = matrixB->cols;
    
    if (matrixA->cols != matrixB->rows)
    {
        printf("matrix a cols (%d) != matrix b rows (%d)\n", matrixA->cols, matrixB->rows);
        return c;
    }


    int n = matrixA->cols;  // общая размерность
    int m = c.rows;         // строки результата
    int p = c.cols;         // столбцы результата
    
    c.data = (double**)malloc(m * sizeof(double*));
    if (c.data == NULL)
    {
        return c;
    }
    for (int i = 0; i < m; i++)
    {
        c.data[i] = (double*)calloc(p, sizeof(double));
        if (c.data[i] == NULL) 
        {
            for (int k = 0; k < i; k++)
                free(c.data[k]);
            free(c.data);
            c.data = NULL;
            return c;
        }
    }
    
    switch (mode)
    {
        case I_J_K:
            for (int i = 0; i < m; i++)
                for (int j = 0; j < p; j++)
                    for (int k = 0; k < n; k++)
                        c.data[i][j] += matrixA->data[i][k] * matrixB->data[k][j];
            break;
            
        case I_K_J:
            for (int i = 0; i < m; i++)
                for (int k = 0; k < n; k++)
                {
                    double aik = matrixA->data[i][k];
                    for (int j = 0; j < p; j++)
                        c.data[i][j] += aik * matrixB->data[k][j];
                }
            break;
            
        case K_I_J:
            for (int k = 0; k < n; k++)
                for (int i = 0; i < m; i++)
                {
                    double aik = matrixA->data[i][k];
                    for (int j = 0; j < p; j++)
                        c.data[i][j] += aik * matrixB->data[k][j];
                }
            break;
            
        case K_J_I:
            for (int k = 0; k < n; k++)
                for (int j = 0; j < p; j++)
                {
                    double bkj = matrixB->data[k][j];
                    for (int i = 0; i < m; i++)
                        c.data[i][j] += matrixA->data[i][k] * bkj;
                }
            break;
            
        case J_I_K:
            for (int j = 0; j < p; j++)
                for (int i = 0; i < m; i++)
                {
                    double sum = 0;
                    for (int k = 0; k < n; k++)
                        sum += matrixA->data[i][k] * matrixB->data[k][j];
                    c.data[i][j] = sum;
                }
            break;
            
        case J_K_I:
            for (int j = 0; j < p; j++)
                for (int k = 0; k < n; k++)
                {
                    double bkj = matrixB->data[k][j];
                    for (int i = 0; i < m; i++)
                        c.data[i][j] += matrixA->data[i][k] * bkj;
                }
            break;
            
        default:
            printf("Unknown multiplication mode\n");
            break;
    }

    return c;
}

bool WriteToFile(const Matrix* matrix, const char* filename, const bool isAdding)
{
    if (matrix == NULL || matrix->data == NULL)
    {
        printf("Ошибка: матрица пуста\n");
        return false;
    }
    
    char* mode = isAdding == true ? "a+": "w";

    FILE* fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return false;
    }
    
    fprintf(fp, "=======================================================\n");
    fprintf(fp, "%d %d\n", matrix->rows, matrix->cols);
    

    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            fprintf(fp, "%.2f", matrix->data[i][j]);
            if (j < matrix->cols - 1)
                fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }
    
    fprintf(fp, "\n");
    fclose(fp);
    return true;
}

#pragma endregion


#pragma region private methods implementation

double** ReadMatrixFromFile(const char* filename, int* rows, int* columns)
{
    char buffer[256];
    FILE *fp = fopen(filename, "r");
    
    if(!fp) {
        printf("file %s doesn't exists!\n", filename);
        return NULL;
    }
    
    double **matrix = NULL;
    *rows = 0;
    *columns = 0;
    
    while(fgets(buffer, 256, fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        
        int curr_cols = 0;
        char temp_buffer[256];
        strcpy(temp_buffer, buffer);
        
        //count cols in row
        char *token = strtok(temp_buffer, " \t");
        while(token != NULL)
        {
            curr_cols++;
            token = strtok(NULL, " \t");
        }
        
        if(curr_cols == 0) continue;
        
        //set cols in first row
        if(*rows == 0) *columns = curr_cols;
        
        //realloc on every row
        matrix = (double**)realloc(matrix, (*rows + 1) * sizeof(double*));

        if (matrix == NULL)
        {
            return NULL;
        }


        matrix[*rows] = (double*)malloc(*columns * sizeof(double));
        if (matrix[*rows] == NULL)
        {
            return NULL;
        }
        
        //set matrix data
        token = strtok(buffer, " \t");
        for(int j = 0; j < *columns; j++)
        {
            if(token)
                //ascii to double
                matrix[*rows][j] = atof(token);
            else
                matrix[*rows][j] = 0.0;
            
            token = token ? strtok(NULL, " \t") : NULL;
        }
        
        (*rows)++;
    }
    
    fclose(fp);
    return matrix;
}
#pragma endregion
