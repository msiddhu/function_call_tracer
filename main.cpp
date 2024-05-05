#include <iostream>
#include <vector>
#include <thread>
#include <__random/random_device.h>
#include "matrix_thread.h"
#include "matrix_event.h"

using namespace std;

/*
 * CPU-Intensive: Matrix Multiplication
 * Memory-Intensive: In-memory databases (get a great in-memory database course here)
 * I/O-Intensive: Databases, File systems
 */

void performMatrixMultiplication() {
    vector<int> matrixSizes = {1,10,100,1000,2000};
    for(int matrixSize : matrixSizes) {
        stopThreads = false;
        cout << "Matrix Size: " << matrixSize<<endl;

        Matrix a = generateMatrix(matrixSize, matrixSize);
        Matrix b = generateMatrix(matrixSize, matrixSize);

        auto startTime = chrono::high_resolution_clock::now();
        Matrix resultThread = multiplyMatricesUsingThreads(a, b);
        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
        cout << " Threaded: " << elapsedTime.count() << " ms"<<endl;

        Matrix resultEvent(a.size(), vector<int>(b[0].size(), 0));
        tasksRemaining = matrixSize * matrixSize;

        startTime = chrono::high_resolution_clock::now();
        thread listener(listenerThread, ref(a), ref(b), ref(resultEvent));
        thread worker(computationThread, ref(a), ref(b), ref(resultEvent));

        while(tasksRemaining > 0) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }

        stopThreads = true;
        taskAvailable.notify_all();
        listener.join();
        worker.join();

        elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
        cout << " Coroutines: " << elapsedTime.count() << " ms"<<endl;
        cout<<endl;
    }
}

void performKeyValueStoreOperations



int main() {
    performMatrixMultiplication();
    return 0;

}
