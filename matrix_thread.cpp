#include <thread>
#include "common.h"

using namespace std;

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



int main() {
    cout<<"Matrix Multiplication using Threads"<<endl;
    int matrixSizes[] = {1,10,100,1000,2000};
    int size = sizeof(matrixSizes) / sizeof(matrixSizes[0]);

    for(int i = 0; i < size; ++i) {
        int matrixSize = matrixSizes[i];
        int** a = generateMatrix(matrixSize, matrixSize);
        int** b = generateMatrix(matrixSize, matrixSize);

        auto start = chrono::high_resolution_clock::now();
        int** resultThread = multiplyMatricesUsingThreads(a, b, matrixSize, matrixSize, matrixSize);
        auto end = chrono::high_resolution_clock::now();
        cout <<"Matrix Size: "<<matrixSize<< " Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

        // Don't forget to deallocate the memory
        for(int j = 0; j < matrixSize; ++j) {
            delete[] a[j];
            delete[] b[j];
            delete[] resultThread[j];
        }
        delete[] a;
        delete[] b;
        delete[] resultThread;
    }
    return 0;
}