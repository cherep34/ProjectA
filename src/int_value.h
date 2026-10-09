#pragma once

#include "ivalue.h"
#include <cstdint>

class IntValue : public IValue {
    std::int64_t value_;
public:
    explicit IntValue(std::int64_t v) : value_(v) {}
    
    std::string to_string() const override { return std::to_string(value_); }
    
    std::unique_ptr<IValue> clone() const override {
        return std::make_unique<IntValue>(value_);
    }
    
    bool is_int() const override { return true; }
    
    std::int64_t get() const { return value_; }
    void set(std::int64_t v) { value_ = v; }
};