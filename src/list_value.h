#pragma once

#include "ivalue.h"
#include <vector>
#include <memory>

class ListValue : public IValue {
    std::vector<std::unique_ptr<IValue>> items_;
public:
    ListValue() = default;
    
    void add(std::unique_ptr<IValue> item) {
        items_.push_back(std::move(item));
    }
    
    std::size_t size() const { return items_.size(); }
    
    const IValue& get(std::size_t index) const {
        return *items_.at(index);
    }
    
    std::string to_string() const override {
        std::string result = "[";
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (i > 0) result += ", ";
            result += items_[i]->to_string();
        }
        result += "]";
        return result;
    }
    
    std::unique_ptr<IValue> clone() const override {
        auto copy = std::make_unique<ListValue>();
        for (const auto& item : items_) {
            copy->add(item->clone());
        }
        return copy;
    }
    
    bool is_list() const override { return true; }
};