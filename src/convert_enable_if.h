#pragma once

#include <string>
#include <string_view>
#include <charconv>
#include <type_traits>
#include <system_error>
#include <stdexcept>



// 1. Для string_view
template<typename T>
std::enable_if_t<std::is_same_v<T, std::string_view>, T>
convert_enable_if(std::string_view sv) {
    return sv;
}

// 2. Для string
template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, T>
convert_enable_if(std::string_view sv) {
    return std::string(sv);
}

// 3. Для арифметических типов
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, T>
convert_enable_if(std::string_view sv) {
    T result{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);
    if (ec != std::errc{}) {
        throw std::runtime_error("Cannot convert '" + std::string(sv) + "' to number");
    }
    return result;
}

// 4. Catch-all для неподдерживаемых типов (чтобы дать понятную ошибку вместо простыни)
template<typename T>
std::enable_if_t<
    !std::is_same_v<T, std::string_view> &&
    !std::is_same_v<T, std::string> &&
    !std::is_arithmetic_v<T>,
    T>
convert_enable_if(std::string_view /*sv*/) {
    static_assert(sizeof(T) == 0, 
        "Unsupported type for convert_enable_if. "
        "Only string_view, string, and arithmetic types are supported.");
    return T{}; 
}