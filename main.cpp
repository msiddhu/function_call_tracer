#include <iostream>
#include <vector>
#include <thread>
#include <__random/random_device.h>
#include "matrix_thread.h"
#include "matrix_event.h"
#include "key_value_store.h"

using namespace std;

/*
 * CPU-Intensive: Matrix Multiplication
 * Memory-Intensive: In-memory databases (get a great in-memory database course here)
 * I/O-Intensive: Databases, File systems
 */

void performMatrixMultiplication() {
    vector<int> matrixSizes = {1,10,100,1000,2000};
    for(int matrixSize : matrixSizes) {

        cout << "Matrix Size: " << matrixSize<<endl;

        Matrix a = generateMatrix(matrixSize, matrixSize);
        Matrix b = generateMatrix(matrixSize, matrixSize);

        auto startTime = chrono::high_resolution_clock::now();
        Matrix resultThread = multiplyMatricesUsingThreads(a, b);
        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
        cout << " Threaded: " << elapsedTime.count() << " ms"<<endl;

        Matrix resultEvent(a.size(), vector<int>(b[0].size(), 0));

        startTime = chrono::high_resolution_clock::now();
        perform_matrix_multiplication_using_coroutines(a, b, resultEvent);
        elapsedTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
        cout << " Coroutines: " << elapsedTime.count() << " ms"<<endl;
        cout<<endl;
    }
}

//void performKeyValueStoreOperations(){
//    key_value_store keyValueStore;
//    keyValueStore.set("key1", "value1");
//    keyValueStore.set("key2", "value2");
//    keyValueStore.set("key3", "value3");
//
//    cout << "Value for key1: " << keyValueStore.get("key1") << endl;
//    cout << "Value for key2: " << keyValueStore.get("key2") << endl;
//    cout << "Value for key3: " << keyValueStore.get("key3") << endl;
//
//    keyValueStore.remove("key2");
//    cout << "Value for key2 after removal: " << keyValueStore.get("key2") << endl;
//
//}



int main() {
    performMatrixMultiplication();
    return 0;

}
