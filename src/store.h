#pragma once

#include <map>
#include <string>
#include <optional>
#include "value.h"

class Store {
private:
    std::map<std::string, Value> store;
public:
    void set(const std::string& key, Value value);
    std::optional<Value> get(const std::string& key) const; // Этап 2: optional вместо сырого указателя
    bool del(const std::string& key);
    std::size_t size() const;
    
    template <typename F>
    void for_each(F&& fn) const {
        for (const auto& [k, v] : store) {
            fn(k, v);
        }
    }
};