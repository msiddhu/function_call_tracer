#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include "matrix_event.h"

using namespace std;
using Matrix = std::vector<std::vector<int>>;

std::queue<Task> taskQueue;
std::mutex queueMutex;
std::condition_variable taskAvailable;
std::atomic<bool> stopThreads(false);
std::condition_variable computationComplete;
std::mutex computationMutex;
std::atomic<int> tasksRemaining;

void listenerThread(const Matrix &a, const Matrix &b, Matrix &result) {
    int currentTaskRow = 0;
    int currentTaskCol = 0;

    // Check for valid matrix dimensions
    if(a[0].size() != b.size()) {
        cout << "Matrix dimensions are not compatible for multiplication\n";
        return;
    }

    // Check for tasks to compute
    if(tasksRemaining == 0) {
        cout << "No tasks to compute\n";
        return;
    }

    // Generate tasks and wait for their completion
    while (!stopThreads && (currentTaskRow < a.size() || !taskQueue.empty())) {
        // Generate task
        {
            lock_guard<mutex> lock(queueMutex);
            taskQueue.push({currentTaskRow, currentTaskCol});
        }
        taskAvailable.notify_one();

        // Wait for task completion
        {
            unique_lock<mutex> lock(computationMutex);
            computationComplete.wait(lock, []{ return taskQueue.empty(); });
        }

        // Move to the next task
        currentTaskCol++;
        if (currentTaskCol == b[0].size()) {
            currentTaskCol = 0;
            currentTaskRow++;
        }
    }
}

void computationThread(const Matrix &a, const Matrix &b, Matrix &result) {
    // Compute tasks until there are no tasks remaining
    while (tasksRemaining > 0 || !taskQueue.empty()) {
        Task task;
        // Get task
        {
            unique_lock<mutex> lock(queueMutex);
            taskAvailable.wait(lock, []{ return !taskQueue.empty() || stopThreads; });
            if (stopThreads) break;
            task = taskQueue.front();
            taskQueue.pop();
        }

        // Perform calculation
        result[task.row][task.col] = 0; // Initialize cell first
        for (int k = 0; k < a[0].size(); ++k) {
            result[task.row][task.col] += a[task.row][k] * b[k][task.col];
        }

        // Notify task completion
        {
            lock_guard<mutex> lock(computationMutex);
            computationComplete.notify_one();
        }
        tasksRemaining--; // Decrement after successful computation
    }
}