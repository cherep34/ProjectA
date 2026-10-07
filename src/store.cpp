#include "store.h"
#include <utility>

void Store::set(const std::string& key, Value value) {
    store.insert_or_assign(key, std::move(value));
}

std::optional<Value> Store::get(const std::string& key) const {
    if (auto it = store.find(key); it != store.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool Store::del(const std::string& key) {
    return store.erase(key) > 0;
}

std::size_t Store::size() const {
    return store.size();
}

void Store::mset(const std::vector<std::pair<std::string, Value>>& pairs) {
    Store temp = *this;
    

    for (const auto& [key, value] : pairs) {
        temp.set(key, value);
    }
    
    swap(temp);
    
}