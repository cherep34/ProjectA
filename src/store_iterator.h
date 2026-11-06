#pragma once

#include <iterator>
#include <string>
#include <map>
#include <memory>
#include "ivalue.h"

class StoreIterator {
private:
    using MapType = std::map<std::string, std::unique_ptr<IValue>>;
    using IteratorType = MapType::const_iterator;
    
    IteratorType it_;
    
public:
    // Типы для итератора
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::string*;
    using reference = const std::string&;
    
    StoreIterator() = default;
    explicit StoreIterator(IteratorType it) : it_(it) {}
    
    reference operator*() const {
        return it_->first;
    }
    
    pointer operator->() const {
        return &(it_->first);
    }
    
    StoreIterator& operator++() {
        ++it_;
        return *this;
    }
    
    StoreIterator operator++(int) {
        StoreIterator tmp = *this;
        ++it_;
        return tmp;
    }
    
    bool operator==(const StoreIterator& other) const {
        return it_ == other.it_;
    }
    
    bool operator!=(const StoreIterator& other) const {
        return it_ != other.it_;
    }
};

// Проверка концепции
static_assert(std::forward_iterator<StoreIterator>, 
    "StoreIterator must satisfy forward_iterator concept");