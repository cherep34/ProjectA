#pragma once

#include <map>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include "value.h"

class Store {
public:
    void set(const std::string& key, Value value);
    std::optional<Value> get(const std::string& key) const;
    bool del(const std::string& key);
    std::size_t size() const;
    
    // Атомарная установка нескольких ключей
    void mset(const std::vector<std::pair<std::string, Value>>& pairs);
    
    template <typename F>
    void for_each(F&& fn) const {
        for (const auto& [k, v] : store) {
            fn(k, v);
        }
    }
private:
    std::map<std::string, Value> store;
    
    void swap(Store& other) noexcept {
        std::swap(store, other.store);
    }
    
};