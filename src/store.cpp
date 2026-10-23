#include "store.h"
#include <utility>

void Store::mset(const std::vector<std::pair<std::string, std::unique_ptr<IValue>>>& pairs) {
    Store temp;
    temp.store = std::map<std::string, std::unique_ptr<IValue>>();
    
    for (const auto& [key, value] : store) {
        temp.store[key] = value->clone();
    }
    
    for (const auto& [key, value] : pairs) {
        temp.store[key] = value->clone();
    }
    
    swap(temp);
}