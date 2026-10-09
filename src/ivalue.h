#pragma once

#include <string>
#include <memory>
#include <vector>

class IValue {
public:
    virtual ~IValue() = default;
    
    // Преобразование в строку для вывода
    virtual std::string to_string() const = 0;
    
    // Клонирование (для копирования значений)
    virtual std::unique_ptr<IValue> clone() const = 0;
    
    // Проверка типа
    virtual bool is_string() const { return false; }
    virtual bool is_int() const { return false; }
    virtual bool is_double() const { return false; }
    virtual bool is_list() const { return false; }
};