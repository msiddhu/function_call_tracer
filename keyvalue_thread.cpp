#include "keyvalue.h"
#include <thread>
#include "common.h"
#include <vector>
#include <random>

int main() {
    keyvalue kv;
    std::vector<std::thread> threads;
    vector<Operation> operations;
    operations = generateWorkload(100000, 0.5, 0.4); // 10,000 operations

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

    for (auto& thread : threads) {
        thread.join();
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms" << endl;

    return 0;
}