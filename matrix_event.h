//
// Created by Siddhartha Malladi on 5/3/24.
//

#ifndef OSPROJECT_MATRIX_EVENT_H
#define OSPROJECT_MATRIX_EVENT_H


#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>

using Matrix = std::vector<std::vector<int>>;

struct Task {
    int row, col;
};

extern std::queue<Task> taskQueue;
extern std::mutex queueMutex;
extern std::condition_variable taskAvailable;
extern std::atomic<bool> stopThreads;
extern std::condition_variable computationComplete; // For signaling task completion
extern std::mutex computationMutex;
extern std::atomic<int> tasksRemaining;


void listenerThread(const Matrix &a, const Matrix &b, Matrix &result);
void computationThread(const Matrix &a, const Matrix &b, Matrix &result);


#endif //OSPROJECT_MATRIX_EVENT_H
