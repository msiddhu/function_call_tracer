//
// Created by Siddhartha Malladi on 5/3/24.
//

#ifndef OSPROJECT_MATRIX_THREAD_H
#define OSPROJECT_MATRIX_THREAD_H

#include <iostream>
#include <vector>
#include <thread>
#include <__random/random_device.h>
#include <__random/mersenne_twister_engine.h>
using namespace std;

typedef std::vector<std::vector<int>> Matrix;
Matrix generateMatrix(int rows, int cols);
void multiplyMatrixBlock(const Matrix &a, const Matrix &b, Matrix &result, int startRow, int endRow);
Matrix multiplyMatricesUsingThreads(const Matrix &a, const Matrix &b);



#endif //OSPROJECT_MATRIX_THREAD_H
