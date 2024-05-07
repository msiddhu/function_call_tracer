#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <coroutine>

using namespace std;


enum class OperationType {
    WRITE,
    READ,
    SEARCH
};

struct Workload {
    OperationType type;
    int line{};
    std::string searchTerm;
};

std::vector<Workload> generateWorkload(int numOperations) {
    std::vector<Workload> workload;
    workload.reserve(numOperations);

    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000);

    for (int i = 0; i < numOperations; ++i) {
        Workload op;
        op.line = dis(gen);
        switch (i % 3) {
            case 0:
                op.type = OperationType::WRITE;
                break;
            case 1:
                op.type = OperationType::READ;
                break;
            case 2:
                op.type = OperationType::SEARCH;
                op.searchTerm = "String " + std::to_string(i);
                break;
        }
        workload.push_back(op);
    }


    // count the number of times each operation type appears
    int writeCount = 0;
    int readCount = 0;
    int searchCount = 0;
    for (const auto& op : workload) {
        switch (op.type) {
            case OperationType::WRITE:
                writeCount++;
                break;
            case OperationType::READ:
                readCount++;
                break;
            case OperationType::SEARCH:
                searchCount++;
                break;
        }
    }
    std::cout << "Write: " << writeCount << ", Read: " << readCount << ", Search: " << searchCount << std::endl;

    std::shuffle(workload.begin(), workload.end(), gen);


    return workload;
}

void writeToFile(std::fstream& file, int line) {
    file.clear();
    if(!file.is_open()){
        std::cerr << "File is not open." << std::endl;
        return;
    }
    if(file.bad()){
        file.clear();
    }
    file << "String " << line << '\n';
    if (file.fail()) {
        std::cerr << "Failed to write to file." << std::endl;
    }
    file.flush();
}

void readFromFile(std::fstream& file, int line) {
    std::string word;
    int lineNumber = 0;
    file.seekg(0);
    char ch;
    if(file.bad()){
        std::cerr << "File is bad." << std::endl;
        file.clear();
    }
    while (file.get(ch)) {
        if (ch != '\n') {
            word += ch;
        } else {
            lineNumber++;
            if (lineNumber == line) {
                //   std::cout << "Reading: " << word << std::endl;
            }
            word.clear();
        }
    }
}


//searches full file for word only first occuring element is printed
void searchInFile(std::fstream& file, const std::string& searchTerm) {
    std::string line;
    int lineNumber = 0;
    file.seekg(0);
    char ch;
    while (file.get(ch)) {
        if (ch != '\n') {
            line += ch;
        } else {
            lineNumber++;
            if (line.find(searchTerm) != std::string::npos) {
            //    std::cout << "Found '" << searchTerm << "' at line " << lineNumber << std::endl;
                break;
            }
            line.clear();
        }
    }
}

struct ExecuteOperation {
    struct promise_type {
        Workload workload;
        auto get_return_object() { return ExecuteOperation{handle_type::from_promise(*this)}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() { }
        void unhandled_exception() { std::exit(1); }
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    ExecuteOperation(handle_type h) : coro(h) { }
    ExecuteOperation(ExecuteOperation&) = delete;
    ExecuteOperation(ExecuteOperation&&) = delete;
    ~ExecuteOperation() { coro.destroy(); }

    void start() { coro.resume(); }
};

ExecuteOperation executeOperation(std::fstream& file, const Workload& workload) {
    switch (workload.type) {
        case OperationType::WRITE:
            writeToFile(file, workload.line);
            break;
        case OperationType::READ:
            readFromFile(file, workload.line);
            break;
        case OperationType::SEARCH:
            searchInFile(file, workload.searchTerm);
            break;
    }
    co_return;
}

int main() {
    std::fstream file("test.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return -1;
    }
    cout<<"File Operations Coroutine-based"<<endl;
    int numOperationSizes[]={5,10,100,500,1000,5000,10000,50000};
    for(int i=0;i<sizeof(numOperationSizes)/sizeof(numOperationSizes[0]);i++) {
        file.open("test.txt", std::ios::out | std::ios::trunc);
        int numOperations = numOperationSizes[i];
        std::vector<Workload> workload = generateWorkload(numOperations);
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& work : workload) {
            ExecuteOperation op = executeOperation(file, work);
            op.start();
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        std::cout << "Operations: "<<numOperations<<" Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms"
                  << std::endl;
        cout<<endl;
    }
    file.close();

    return 0;
}