#pragma once

#include "ivalue.h"
#include <deque>
#include <string>
#include <vector>
#include <stdexcept>

class ListValue : public IValue {
private:
    std::deque<std::string> data;

public:
    ListValue() = default;
    
    // Вставка в начало
    void push_front(const std::string& value) {
        data.push_front(value);
    }
    
    // Вставка в конец
    void push_back(const std::string& value) {
        data.push_back(value);
    }
    
    // Извлечение из начала
    std::string pop_front() {
        if (data.empty()) throw std::runtime_error("List is empty");
        std::string value = data.front();
        data.pop_front();
        return value;
    }
    
    // Извлечение из конца
    std::string pop_back() {
        if (data.empty()) throw std::runtime_error("List is empty");
        std::string value = data.back();
        data.pop_back();
        return value;
    }
    
    // Получение диапазона 
    std::vector<std::string> range(int start, int stop) const {
        int size = static_cast<int>(data.size());
        
        // Нормализация отрицательных индексов
        if (start < 0) start = size + start;
        if (stop < 0) stop = size + stop;
        
        // Граничные условия
        if (start < 0) start = 0;
        if (stop >= size) stop = size - 1;
        if (start > stop) return {};
        
        std::vector<std::string> result;
        for (int i = start; i <= stop; ++i) {
            result.push_back(data[i]);
        }
        return result;
    }
    
    std::size_t length() const {
        return data.size();
    }
    
    std::string to_string() const override {
        std::string result = "[";
        for (std::size_t i = 0; i < data.size(); ++i) {
            if (i > 0) result += ", ";
            result += "\"" + data[i] + "\"";
        }
        result += "]";
        return result;
    }
    
    bool is_string() const override { return false; }
    bool is_int() const override { return false; }
    bool is_double() const override { return false; }
    bool is_list() const override { return true; }
    
    std::unique_ptr<IValue> clone() const override {
        auto copy = std::make_unique<ListValue>();
        copy->data = data;
        return copy;
    }
};