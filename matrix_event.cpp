//
// Created by Siddhartha Malladi on 5/5/24.
//
#include <coroutine>
#include <iostream>
#include "common.h"


using namespace std;

struct Task {
    int row, col, k;
};

struct TaskGenerator {
    struct promise_type {
        Task currentTask;
        auto get_return_object() { return TaskGenerator{this}; }

        //initially this is suspended state.
        auto initial_suspend() { return suspend_always{}; }

        //final suspend is also suspended state
        auto final_suspend() noexcept { return suspend_always{}; }

        // kill the coroutine if exception is thrown
        void unhandled_exception() { std::terminate(); }

        //
        void return_void() {}

        //
        auto yield_value(Task value) {
            currentTask = value;
            return suspend_always{};
        }
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;


    //constructor
    TaskGenerator(promise_type* p)
            : coro(handle_type::from_promise(*p)) {}

    //destructor
    ~TaskGenerator() {
        if (coro) coro.destroy();
    }

    //wrapper function to resume the coroutine
    bool next() {
        if (!coro.done()) {
            coro.resume();
            return true;
        }
        return false;
    }

    //get the current task
    Task getValue() {
        return coro.promise().currentTask;
    }
};

TaskGenerator generateTasks(int rows, int cols, int commonDimension) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < commonDimension; ++k) {
                co_yield Task{i, j, k};
            }
        }
    }
}

void perform_matrix_multiplication_using_coroutines(int** a, int** b, int** result, int rows, int cols, int commonDimension) {
    TaskGenerator tasks = generateTasks(rows, cols, commonDimension);
    while (tasks.next()) {
        //blocking call

        Task task = tasks.getValue();
        result[task.row][task.col] += a[task.row][task.k] * b[task.k][task.col];
    }
}

int main() {
    cout<<"Matrix Multiplication using Coroutines"<<endl;
//    int matrixSizes[] = {1,10,100,1000,2000};
    int matrixSizes[] = {1,10,100,1000,2000};
    int size = sizeof(matrixSizes) / sizeof(matrixSizes[0]);
    for(int i=0; i<size; i++){
        int matrixSize = matrixSizes[i];
        int** a = generateMatrix(matrixSize, matrixSize);
        int** b = generateMatrix(matrixSize, matrixSize);
        int** resultEvent = new int*[matrixSize];
        for(int j = 0; j < matrixSize; ++j) {
            resultEvent[j] = new int[matrixSize]();
        }
        auto start= chrono::high_resolution_clock::now();
        perform_matrix_multiplication_using_coroutines(a, b, resultEvent, matrixSize, matrixSize, matrixSize);
        auto end = chrono::high_resolution_clock::now();
        cout << "Matrix Size: "<<matrixSize<<" Time taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

        cout<<endl;
        for(int j = 0; j < matrixSize; ++j) {
            delete[] a[j];
            delete[] b[j];
            delete[] resultEvent[j];
        }
        delete[] a;
        delete[] b;
        delete[] resultEvent;

    }

    return 0;
}


