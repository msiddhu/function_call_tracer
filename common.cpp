//
// Created by Siddhartha Malladi on 5/5/24.
//

#include "common.h"
#include <string>

using namespace std;



std::vector<Operation> generateWorkload(int numOperations, double readRatio, double writeRatio) {
    std::vector<Operation> workload;
    workload.reserve(numOperations); // Pre-allocate for efficiency

    // Custom random number generator
    auto randCustom = []() -> int {
        static unsigned long x = 123456789, y = 362436069, z = 521288629;
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;
        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;
        return z % 1000 + 1; // Returns a number between 1 and 1000
    };

    // Custom uniform distribution
    auto uniformDist = [&randCustom](double min, double max) -> double {
        return min + (max - min) * (double(randCustom()) / 1000.0);
    };

    for (int i = 0; i < numOperations; ++i) {
        double opType = uniformDist(0.0, 1.0);
        std::string key = std::to_string(randCustom());

        if (opType <= readRatio) {
            workload.push_back({ OperationType::GET, key, "" });
        } else if (opType <= readRatio + writeRatio) {
            workload.push_back({ OperationType::SET, key, "some_value" });
        } else if (opType <= readRatio + writeRatio + 0.1) { // 10% DEL
            workload.push_back({ OperationType::DEL, key, "" });
        } else {
            workload.push_back({ OperationType::RANGE_QUERY, "key200", "key700" });
        }
    }

    return workload;
}
// Custom random number generator
static int randCustom() {
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
            matrix[i][j] = randCustom();
        }
    }
    return matrix;
}



