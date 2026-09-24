#pragma once

#include <string>
#include <variant>
#include <cstdint>
#include <cstddef>
#include <utility>

class TrackedString {
private:
    std::string str;
public:
    inline static size_t copy_ctor_count = 0;
    inline static size_t move_ctor_count = 0;

    TrackedString() = default;
    TrackedString(std::string s) : str(std::move(s)) {}

    TrackedString(const TrackedString& other) : str(other.str) { ++copy_ctor_count; }
    TrackedString(TrackedString&& other) noexcept : str(std::move(other.str)) { ++move_ctor_count; }
    
    TrackedString& operator=(const TrackedString& other) { str = other.str; return *this; }
    TrackedString& operator=(TrackedString&& other) noexcept { str = std::move(other.str); return *this; }

    const std::string& get() const { return str; }
    std::string to_string() const { return str; }
};

using Value = std::variant<TrackedString, std::int64_t, double>;

namespace ValueTracking {
    inline void reset_counters() {
        TrackedString::copy_ctor_count = 0;
        TrackedString::move_ctor_count = 0;
    }
    inline size_t get_copy_ctor_count() { return TrackedString::copy_ctor_count; }
    inline size_t get_move_ctor_count() { return TrackedString::move_ctor_count; }
}