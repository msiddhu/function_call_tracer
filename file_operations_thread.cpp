#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>

#define NUM_THREADS 64


using namespace std;
std::mutex mtx;

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
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
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
               // std::cout << "Found '" << searchTerm << "' at line " << lineNumber << std::endl;
                break;
            }
            line.clear();
        }
    }
}

void executeOperation(std::fstream& file, const Workload& workload) {
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
}

int main() {
    std::fstream file("test.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return -1;
    }

    cout<<"File Operations Thread-based"<<endl;
    cout << "Number of threads: " << NUM_THREADS << endl;
    int numOperationSizes[]={5,10,100,500,1000,5000,10000,50000};
    for(int i=0;i<sizeof(numOperationSizes)/sizeof(numOperationSizes[0]);i++) {
        file.open("test.txt", std::ios::out | std::ios::trunc);
        int numOperations = numOperationSizes[i];
        int operationsPerThread = numOperations / NUM_THREADS;
        std::vector<Workload> workload = generateWorkload(numOperations);
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        for (int t = 0; t < NUM_THREADS; ++t) {
            int startOp = t * operationsPerThread;
            int endOp = (t == NUM_THREADS - 1) ? numOperations : startOp + operationsPerThread;
            threads.emplace_back([&file, &workload, startOp, endOp]() {
                for (int i = startOp; i < endOp; ++i) {
                    executeOperation(file, workload[i]);
                }
            });
        }

        for (auto &thread: threads) {
            thread.join();
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        std::cout << "Operations: "<<numOperations<<" Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms"
                  << std::endl;
        cout<<endl;
    }
    // if file is not closed there could be a memory leak.
    file.close();

    return 0;
}