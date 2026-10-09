#include "store.h"
#include <utility>

void Store::set(const std::string& key, std::unique_ptr<IValue> value) {
    store.insert_or_assign(key, std::move(value));
}

std::optional<IValue*> Store::get(const std::string& key) const {
    if (auto it = store.find(key); it != store.end()) {
        return it->second.get();
    }
    return std::nullopt;
}

bool Store::del(const std::string& key) {
    return store.erase(key) > 0;
}

std::size_t Store::size() const {
    return store.size();
}

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