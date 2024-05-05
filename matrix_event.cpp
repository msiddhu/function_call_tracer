#include <coroutine>
#include <iostream>
#include <vector>

using namespace std;
using Matrix = vector<vector<int>>;

struct Task {
    int row, col;
};

struct TaskGenerator {
    struct promise_type {
        Task currentTask;
        auto get_return_object() { return TaskGenerator{this}; }
        auto initial_suspend() { return suspend_always{}; }
        auto final_suspend() noexcept { return suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
        auto yield_value(Task value) {
            currentTask = value;
            return suspend_always{};
        }
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    TaskGenerator(promise_type* p)
            : coro(handle_type::from_promise(*p)) {}

    ~TaskGenerator() {
        if (coro) coro.destroy();
    }

    bool next() {
        if (!coro.done()) {
            coro.resume();
            return true;
        }
        return false;
    }

    Task getValue() {
        return coro.promise().currentTask;
    }
};

TaskGenerator generateTasks(int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            co_yield Task{i, j};
        }
    }
}

void perform_matrix_multiplication_using_coroutines(const Matrix &a, const Matrix &b, Matrix &result) {
    TaskGenerator tasks = generateTasks(a.size(), b[0].size());
    while (tasks.next()) {
        Task task = tasks.getValue();
        result[task.row][task.col] = 0;
        for (int k = 0; k < a[0].size(); ++k) {
            result[task.row][task.col] += a[task.row][k] * b[k][task.col];
        }
    }
}