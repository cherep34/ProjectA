#pragma once

#include <cstddef>
#include <string>


inline std::size_t fnv1a_hash(const std::string& key) {
    std::size_t hash = 14695981039346656037ULL; 
    
    for (char c : key) {
        hash ^= static_cast<std::size_t>(c);
        hash *= 1099511628211ULL;             
    }
    
    return hash;
}