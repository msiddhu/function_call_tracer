//
// Created by Siddhartha Malladi on 5/5/24.
//

#ifndef OSPROJECT_KEYVALUE_H
#define OSPROJECT_KEYVALUE_H

#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;


class keyvalue {
public:
    std::string get(const std::string& key) {
        std::lock_guard<std::mutex> lock(storeMutex);
        auto it = store.find(key);
        return (it != store.end()) ? it->second : "";
    }

    void del(const std::string& key) {
        std::lock_guard<std::mutex> lock(storeMutex);
        store.erase(key);
    }

    void rangeQuery(const std::string& startKey, const std::string& endKey) {
        std::lock_guard<std::mutex> lock(storeMutex);
        std::vector<std::string> result;
        for (const auto& pair : store) {
            if (pair.first >= startKey && pair.first <= endKey) {
                result.push_back(pair.second);
            }
        }
//        return result;
    }

    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(storeMutex);
        store[key] = value;
    }

private:
    std::unordered_map<std::string, std::string> store;
    std::mutex storeMutex;
};


#endif //OSPROJECT_KEYVALUE_H
