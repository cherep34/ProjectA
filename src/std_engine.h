#pragma once

#include "istorage_engine.h"
#include "store_iterator.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

class StdEngine : public IStorageEngine {
private:
    std::map<std::string, std::unique_ptr<IValue>> data;

public:
    void put(const std::string& key, std::unique_ptr<IValue> value) override {
        data.insert_or_assign(key, std::move(value));
    }
    
    IValue* get(const std::string& key) const override {
        auto it = data.find(key);
        return (it != data.end()) ? it->second.get() : nullptr;
    }
    
    bool erase(const std::string& key) override {
        return data.erase(key) > 0;
    }
    
    bool contains(const std::string& key) const override {
        return data.count(key) > 0;
    }
    
    std::size_t size() const override {
        return data.size();
    }
    
    StoreIterator begin() const override {
        return StoreIterator(data.cbegin());
    }
    
    StoreIterator end() const override {
        return StoreIterator(data.cend());
    }
    
    void clear() override {
        data.clear();
    }
    
    std::vector<std::string> keys_range(const std::string& from, const std::string& to) const override {
        std::vector<std::string> result;
        auto it_from = data.lower_bound(from);
        auto it_to = data.upper_bound(to);
        for (auto it = it_from; it != it_to; ++it) {
            result.push_back(it->first);
        }
        return result;
    }
};