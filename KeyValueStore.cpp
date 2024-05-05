//
// Created by Siddhartha Malladi on 5/5/24.
//

#include "KeyValueStore.h"

#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>

class KeyValueStore {
public:
    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(storeMutex);
        store[key] = value;
    }

    std::string get(const std::string& key) {
        std::lock_guard<std::mutex> lock(storeMutex);
        auto it = store.find(key);
        return (it != store.end()) ? it->second : "";
    }

    void del(const std::string& key) {
        std::lock_guard<std::mutex> lock(storeMutex);
        store.erase(key);
    }

private:
    std::unordered_map<std::string, std::string> store;
    std::mutex storeMutex;
};

