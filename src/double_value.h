#pragma once

#include "ivalue.h"

class DoubleValue : public IValue {
    double value_;
public:
    explicit DoubleValue(double v) : value_(v) {}
    
    std::string to_string() const override { return std::to_string(value_); }
    
    std::unique_ptr<IValue> clone() const override {
        return std::make_unique<DoubleValue>(value_);
    }
    
    bool is_double() const override { return true; }
    
    double get() const { return value_; }
};