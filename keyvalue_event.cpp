#include "keyvalue.h"
#include "common.h"
#include <coroutine>

using namespace std;

struct Task {
    Operation operation;
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

TaskGenerator generateTasks(vector<Operation>& operations) {
    for (auto& operation : operations) {
        co_yield Task{operation};
    }
}

int main() {
    keyvalue kv;
    vector<Operation> operations;
    operations = generateWorkload(100000, 0.1, 0.9);

    // set timers
    auto start = std::chrono::high_resolution_clock::now();

    TaskGenerator tasks = generateTasks(operations);
    while (tasks.next()) {
        Task task = tasks.getValue();
        if (task.operation.type == OperationType::GET) {
            kv.get(task.operation.key);
        } else if (task.operation.type == OperationType::SET) {
            kv.set(task.operation.key, task.operation.value);
        } else if (task.operation.type == OperationType::DEL) {
            kv.del(task.operation.key);
        } else if (task.operation.type == OperationType::RANGE_QUERY) {
            kv.rangeQuery(task.operation.key, task.operation.value);
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms" << endl;

    return 0;
}