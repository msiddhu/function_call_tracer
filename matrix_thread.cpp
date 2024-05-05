//
// Created by Siddhartha Malladi on 5/3/24.
//

#include <thread>
#include "matrix_thread.h"

using namespace std;

// Simple pseudo-random number generator
int rand() {
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z % 100 + 1; // Returns a number between 1 and 100
}

// Generate a random matrix
int** generateMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for(int j = 0; j < cols; ++j) {
            matrix[i][j] = rand();
        }
    }
    return matrix;
}

void multiplyMatrixBlock(int** a, int** b, int** result, int startRow, int endRow, int commonDimension) {
    for(int i = startRow; i < endRow; ++i) {
        for(int j = 0; j < commonDimension; ++j) {
            result[i][j] = 0;
            for(int k = 0; k < commonDimension; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int** multiplyMatricesUsingThreads(int** a, int** b, int rows, int cols, int commonDimension) {
    int** result = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        result[i] = new int[cols];
    }

    unsigned int numThreads = thread::hardware_concurrency(); // Adjust as needed
    int rowsPerThread = rows / numThreads;

    vector<thread> threads;
    for(int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? rows : startRow + rowsPerThread;
        threads.emplace_back(multiplyMatrixBlock, a, b, result, startRow, endRow, commonDimension);
    }

    for(auto &th : threads) {
        th.join();
    }

    return result;
}