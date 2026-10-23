#pragma once

#include <concepts>
#include <string>
#include <string_view>

template<typename T>
concept KeyLike = requires(const T& key) {
    { std::string_view(key) } -> std::same_as<std::string_view>;
    { key == key } -> std::convertible_to<bool>;
    { key != key } -> std::convertible_to<bool>;
};

template<KeyLike K>
std::string_view to_string_view(const K& key) {
    return std::string_view(key);
}