//
// Created by Siddhartha Malladi on 5/5/24.
//

#ifndef OSPROJECT_COMMON_H
#define OSPROJECT_COMMON_H

#include <iostream>
#include <vector>
#include <thread>
using namespace std;



enum class OperationType {
    GET,
    SET,
    DEL,
    RANGE_QUERY
};

struct Operation {
    OperationType type;
    std::string key;
    std::string value; // Used for SET operations
};

int rand();
int** createMatrix(int rows, int cols);
int** generateMatrix(int rows, int cols);
vector<Operation> generateWorkload(int numOperations, double readRatio, double writeRatio);
//
#endif //OSPROJECT_COMMON_H
