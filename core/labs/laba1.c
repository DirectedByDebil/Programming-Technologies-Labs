#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../utils/bool.h"
#include "../utils/inputParams.h"
#include "../utils/ioUtils.h"
#include "../matrix/matrix.h"
#include "laba.h"

const char* TASK_1_RESULTS = "core/labs/task_1.txt";
const char* TASK_2_RESULTS = "core/labs/task_2.txt";


struct TaskResults
{
    Matrix* a;
    Matrix* b;
    Matrix* c;

    double executionTime;
    double flops;
    double gflops;
};

typedef struct TaskResults TaskResults;



Matrix matrixA;
Matrix matrixB;


#pragma region private methods prototypes

void FillMatrixes(Params params, Matrix* a, Matrix* b);

TaskResults Task1(const Matrix* matrixA, const Matrix* matrixB);
TaskResults Task2(const Matrix* matrixA, const Matrix* matrixB);
TaskResults Task3(const Matrix* matrixA, const Matrix* matrixB);

#pragma endregion


int startLab1 (int argc, char* argv[]) 
{

    if (argc < 3)
    {
        printf("Need fileNameA, fileNameB!\n");
        printf("OR -r n (matrix of size n randomly generated)!\n");
        return 1;
    }

    Params params = ReadInputParams(argc, argv);
    
    PrintParams(params);
    

    FillMatrixes(params, &matrixA, &matrixB);
    
    TaskResults res1 = Task1(&matrixA, &matrixB);
    printf("Task 1: %lf seconds\n", res1.executionTime);
    printf("Task 1: %lf flops\n", res1.flops);
    printf("Task 1: %.2lf gflops\n", res1.gflops);

    WriteToFile(res1.a, TASK_1_RESULTS, false);
    WriteToFile(res1.b, TASK_1_RESULTS, true);
    WriteToFile(res1.c, TASK_1_RESULTS, true);
    
    TaskResults res2 = Task2(&matrixA, &matrixB);
    

    FreeMatrix(&matrixA);
    FreeMatrix(&matrixB);
    
    return 0;
}


#pragma region private methods implemention

void FillMatrixes(Params params, Matrix* a, Matrix* b)
{

    if (params.randomSize > 0)
    {
        *a = GenerateRandom(params.randomSize);
        *b = GenerateRandom(params.randomSize);
    }
    else if (params.fileA != NULL && params.fileB != NULL)
    {
        *a = ReadFromFile(params.fileA);
        *b = ReadFromFile(params.fileB);   
    }

}

TaskResults Task1(const Matrix* matrixA, const Matrix* matrixB)
{
    TaskResults res;

    printf("\nTask 1: \n");
    PrintMatrix(matrixA);
    PrintMatrix(matrixB);


    clock_t start = clock();

    Matrix matrixC = MultiplyBy(matrixA, matrixB, I_K_J);
    PrintMatrix(&matrixC);

    clock_t end = clock();
    
    res.executionTime = (double)(end - start) / CLOCKS_PER_SEC;
    res.a = CopyFrom(matrixA);
    res.b = CopyFrom(matrixB);
    res.c = CopyFrom(&matrixC);

    int n = matrixA->cols; 
    int m = matrixC.rows;
    int p = matrixC.cols;

    res.flops = (double)(2*n*m*p)/res.executionTime;
    res.gflops = res.flops / 1e9;
    
    return res;
}


TaskResults Task2(const Matrix* matrixA, const Matrix* matrixB)
{

    TaskResults res;
    Matrix* b_T = Transpose(matrixB);

    printf("\nTask 2: \n");
    PrintMatrix(matrixA);
    PrintMatrix(b_T);

    MatrixMultiply modes[] = {I_J_K, I_K_J, K_I_J, K_J_I, J_I_K, J_K_I};
    const char* mode_names[] = {"I_J_K", "I_K_J", "K_I_J", "K_J_I", "J_I_K", "J_K_I"};
    
    for (int i = 0; i < 6; i++)
    {

        clock_t start = clock();
        
        Matrix matrixC = MultiplyBy(matrixA, b_T, modes[i]);
        clock_t end = clock();
        
        double time = (double)(end - start) / CLOCKS_PER_SEC;
        printf("%s: %lf\n", mode_names[i], time);

        int n = matrixA->cols; 
        int m = matrixC.rows;
        int p = matrixC.cols;

        double flops = (double)(2*n*m*p)/time;
        double gflops = flops / 1e9;
        printf("%s: flops %lf\n", mode_names[i], flops);
        printf("%s: gflops %.2lf\n\n", mode_names[i], gflops);
    }
    
    
    //res.executionTime = (double)(end - start) / CLOCKS_PER_SEC;
    res.a = CopyFrom(matrixA);
    res.b = CopyFrom(b_T);
    //res.c = CopyFrom(&matrixC);

    return res;
}


#pragma endregion