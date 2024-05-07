#include "keyvalue.h"
#include <thread>
#include "common.h"
#include <vector>
#include <random>

int main() {

    int numberOperations[] = {1000, 10000, 100000, 1000000,10000000,100000000};
    cout<< "KeyValue Store using Threads" << endl;
    for(int i = 0; i < sizeof(numberOperations) / sizeof(numberOperations[0]); ++i) {
        int numOperations = numberOperations[i];
        keyvalue kv;


        vector<std::thread> threads;
        vector<Operation> operations;

        operations = generateWorkload(numOperations, 0.5, 0.5);

        // set timers
        auto start = std::chrono::high_resolution_clock::now();

        int numThreads = 10; // Use 10 threads
        int operationsPerThread = operations.size() / numThreads;

        threads.reserve(numThreads);
        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back([&kv, &operations, t, operationsPerThread, &numThreads]() {
                int startOp = t * operationsPerThread;
                int endOp = (t == numThreads - 1) ? operations.size() : startOp + operationsPerThread;

                for (int i = startOp; i < endOp; ++i) {
                    if (operations[i].type == OperationType::GET) {
                        kv.get(operations[i].key);
                    } else if (operations[i].type == OperationType::SET) {
                        kv.set(operations[i].key, operations[i].value);
                    } else if (operations[i].type == OperationType::DEL) {
                        kv.del(operations[i].key);
                    } else if (operations[i].type == OperationType::RANGE_QUERY) {
                        kv.rangeQuery(operations[i].key, operations[i].value);
                    }
                }
            });
        }

        for (auto &thread: threads) {
            thread.join();
        }

        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        cout << "Operations " << numOperations << " Time taken: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms" << endl;
    }
    return 0;
}