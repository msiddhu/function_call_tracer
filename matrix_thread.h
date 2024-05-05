//
// Created by Siddhartha Malladi on 5/3/24.
//

#ifndef OSPROJECT_MATRIX_THREAD_H
#define OSPROJECT_MATRIX_THREAD_H

#include <iostream>
#include <vector>
#include <thread>
using namespace std;

typedef int** Matrix;

int** generateMatrix(int rows, int cols);
void multiplyMatrixBlock(int** a, int** b, int** result, int startRow, int endRow, int commonDimension);
int** multiplyMatricesUsingThreads(int** a, int** b, int rows, int cols, int commonDimension);



#endif //OSPROJECT_MATRIX_THREAD_H
