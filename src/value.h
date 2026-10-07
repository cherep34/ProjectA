#pragma once

#include <string>
#include <variant>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <stdexcept>

class TrackedString {
private:
    std::string str;
public:
    inline static size_t copy_ctor_count = 0;
    inline static size_t move_ctor_count = 0;
    inline static int throw_on_Nth_copy = 0; 

    TrackedString() = default;
    TrackedString(std::string s) : str(std::move(s)) {}

    TrackedString(const TrackedString& other) : str(other.str) { 
        ++copy_ctor_count;
        if (throw_on_Nth_copy > 0 && copy_ctor_count >= (size_t)throw_on_Nth_copy) {
            throw std::runtime_error("Simulated copy failure for strong guarantee test");
        }
    }
    
    TrackedString(TrackedString&& other) noexcept : str(std::move(other.str)) { ++move_ctor_count; }
    
    TrackedString& operator=(const TrackedString& other) { 
        str = other.str; 
        ++copy_ctor_count;
        if (throw_on_Nth_copy > 0 && copy_ctor_count >= (size_t)throw_on_Nth_copy) {
            throw std::runtime_error("Simulated copy failure for strong guarantee test");
        }
        return *this; 
    }
    
    TrackedString& operator=(TrackedString&& other) noexcept { 
        str = std::move(other.str); 
        ++move_ctor_count; 
        return *this; 
    }

    const std::string& get() const { return str; }
    std::string to_string() const { return str; }

    static void reset_counters() {
        copy_ctor_count = 0;
        move_ctor_count = 0;
        throw_on_Nth_copy = 0;
    }
    static size_t get_copy_ctor_count() { return copy_ctor_count; }
    static size_t get_move_ctor_count() { return move_ctor_count; }
};

using Value = std::variant<TrackedString, std::int64_t, double>;