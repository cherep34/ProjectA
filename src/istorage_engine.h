#pragma once

#include <string>
#include <optional>
#include <memory>
#include "ivalue.h"

class StoreIterator;


class IStorageEngine {
public:
    virtual ~IStorageEngine() = default;
    
    virtual void put(const std::string& key, std::unique_ptr<IValue> value) = 0;
    
    virtual IValue* get(const std::string& key) const = 0;
    
    virtual bool erase(const std::string& key) = 0;
    
    virtual bool contains(const std::string& key) const = 0;
    
    virtual std::size_t size() const = 0;
    
    virtual StoreIterator begin() const = 0;
    virtual StoreIterator end() const = 0;
    
    virtual void clear() = 0;

    virtual std::vector<std::string> keys_range(const std::string& from, const std::string& to) const = 0;
};