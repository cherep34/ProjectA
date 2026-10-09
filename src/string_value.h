#pragma once

#include "ivalue.h"
#include <string>

class StringValue : public IValue {
    std::string value_;
public:
    explicit StringValue(std::string v) : value_(std::move(v)) {}
    
    std::string to_string() const override { return "\"" + value_ + "\""; }
    
    std::unique_ptr<IValue> clone() const override {
        return std::make_unique<StringValue>(value_);
    }
    
    bool is_string() const override { return true; }
    
    const std::string& get() const { return value_; }
};