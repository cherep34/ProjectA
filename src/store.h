#pragma once

#include <map>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <memory>
#include "ivalue.h"

class Store {
private:
    std::map<std::string, std::unique_ptr<IValue>> store;
    
    void swap(Store& other) noexcept {
        std::swap(store, other.store);
    }
    
public:
    void set(const std::string& key, std::unique_ptr<IValue> value);
    std::optional<IValue*> get(const std::string& key) const;
    bool del(const std::string& key);
    std::size_t size() const;
    
    void mset(const std::vector<std::pair<std::string, std::unique_ptr<IValue>>>& pairs);
    
    template <typename F>
    void for_each(F&& fn) const {
        for (const auto& [k, v] : store) {
            fn(k, *v);
        }
    }
};