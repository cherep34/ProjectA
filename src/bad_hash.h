#pragma once

#include <cstddef>
#include <string>


inline std::size_t bad_hash(const std::string& key) {
    std::size_t hash = 0;
    for (char c : key) {
        hash += static_cast<std::size_t>(c);
    }
    return hash;
}