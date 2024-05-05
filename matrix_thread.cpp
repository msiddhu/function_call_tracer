//
// Created by Siddhartha Malladi on 5/3/24.
//


#include <vector>
#include <__random/random_device.h>
#include <__random/mersenne_twister_engine.h>

#include <thread>
#include "matrix_thread.h"
using namespace std;



// Generate a random matrix
Matrix generateMatrix(int rows, int cols) {
    Matrix matrix(rows, std::vector<int>(cols, 0));
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(1, 100); // Adjust range as needed

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(generator);
        }
    }
    return matrix;
}

void multiplyMatrixBlock(const Matrix &a, const Matrix &b, Matrix &result, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < b.size(); ++j) {
            for (int k = 0; k < a[0].size(); ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


Matrix multiplyMatricesUsingThreads(const Matrix &a, const Matrix &b) {
    int numRowsA = (int)a.size();
    int numColsB = (int)b[0].size();
    Matrix result(numRowsA, std::vector<int>(numColsB, 0));

    unsigned int numThreads = thread::hardware_concurrency(); // Adjust as needed
  //  cout<<"Number of threads: "<<numThreads<<endl;
    std::vector<std::thread> threads;
    int rowsPerThread = numRowsA / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? numRowsA : startRow + rowsPerThread;
        threads.emplace_back(multiplyMatrixBlock, std::cref(a), std::cref(b), std::ref(result), startRow, endRow);
    }

    for (auto &th : threads) {
        th.join();
    }
    return result;
}