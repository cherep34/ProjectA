#include <gtest/gtest.h>
#include "function_traits.h"
#include <string>

// Обычная функция
int add(int a, int b) { return a + b; }

// Функтор
struct Multiply {
    double operator()(double x, double y) const { return x * y; }
};

// Тест: function_traits для обычной функции
TEST(FunctionTraitsTest, FreeFunction) {
    using traits = function_traits<decltype(&add)>;
    
    static_assert(std::is_same_v<traits::return_type, int>);
    static_assert(traits::arity == 2);
    static_assert(std::is_same_v<traits::arg_type<0>, int>);
    static_assert(std::is_same_v<traits::arg_type<1>, int>);
    
    SUCCEED();
}

// Тест: function_traits для лямбды
TEST(FunctionTraitsTest, Lambda) {
    auto lambda = [](std::string s, std::size_t n) -> bool { return s.size() > n; };
    
    using traits = function_traits<decltype(lambda)>;
    
    static_assert(std::is_same_v<traits::return_type, bool>);
    static_assert(traits::arity == 2);
    static_assert(std::is_same_v<traits::arg_type<1>, std::size_t>);
    static_assert(std::is_same_v<traits::arg_type<1>, std::size_t>);
    
    SUCCEED();
}

// Тест: function_traits для функтора
TEST(FunctionTraitsTest, Functor) {
    using traits = function_traits<Multiply>;
    
    static_assert(std::is_same_v<traits::return_type, double>);
    static_assert(traits::arity == 2);
    static_assert(std::is_same_v<traits::arg_type<0>, double>);
    static_assert(std::is_same_v<traits::arg_type<1>, double>);
    
    SUCCEED();
}

// Тест: удобные алиасы
TEST(FunctionTraitsTest, Aliases) {
    auto lambda = [](int x) -> std::string { return std::to_string(x); };
    
    static_assert(std::is_same_v<function_return_t<decltype(lambda)>, std::string>);
    static_assert(function_arity_v<decltype(lambda)> == 1);
    static_assert(std::is_same_v<function_arg_t<decltype(lambda), 0>, int>);
    
    SUCCEED();
}