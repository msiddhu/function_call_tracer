#include <iostream>
#include "matrix_thread.h"
#include "matrix_event.h"

using namespace std;

void performMatrixMultiplication() {
    int matrixSizes[] = {1,10,100,1000};
    int size = sizeof(matrixSizes) / sizeof(matrixSizes[0]);

    for(int i = 0; i < size; ++i) {
        int matrixSize = matrixSizes[i];

        cout << "Matrix Size: " << matrixSize << endl;

        int** a = generateMatrix(matrixSize, matrixSize);
        int** b = generateMatrix(matrixSize, matrixSize);

//        auto startTime = chrono::high_resolution_clock::now();
        int** resultThread = multiplyMatricesUsingThreads(a, b, matrixSize, matrixSize, matrixSize);
//        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
//        cout << " Threaded: " << elapsedTime.count() << " ms" << endl;

        // Assuming perform_matrix_multiplication_using_coroutines is also modified to use dynamic arrays
        int** resultEvent = generateMatrix(matrixSize, matrixSize);
//        startTime = chrono::high_resolution_clock::now();
        perform_matrix_multiplication_using_coroutines(a, b, resultEvent, matrixSize, matrixSize);
//        elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
//        cout << " Coroutines: " << elapsedTime.count() << " ms" << endl;
//        cout << endl;

        // Don't forget to deallocate the memory
        for(int i = 0; i < matrixSize; ++i) {
            delete[] a[i];
            delete[] b[i];
            delete[] resultThread[i];
            delete[] resultEvent[i];
        }
        delete[] a;
        delete[] b;
        delete[] resultThread;
        delete[] resultEvent;
    }
}

int main() {
    performMatrixMultiplication();
    // performKeyValueStoreOperations(); // Assuming this function is also modified to use dynamic arrays
    return 0;
}