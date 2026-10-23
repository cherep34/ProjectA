#pragma once

#include <type_traits>
#include <tuple>

template<typename F>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);
    
    template<size_t I>
    using arg_type = std::tuple_element_t<I, args_tuple>;
};

template<typename R, typename... Args>
struct function_traits<R(Args...)> : function_traits<R(*)(Args...)> {};

template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);
    
    template<size_t I>
    using arg_type = std::tuple_element_t<I, args_tuple>;
};

template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...)> {
    using return_type = R;
    using args_tuple = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);
    
    template<size_t I>
    using arg_type = std::tuple_element_t<I, args_tuple>;
};

template<typename F>
struct function_traits : function_traits<decltype(&F::operator())> {};


template<typename F>
using function_return_t = typename function_traits<F>::return_type;

template<typename F>
constexpr size_t function_arity_v = function_traits<F>::arity;

template<typename F, size_t I>
using function_arg_t = typename function_traits<F>::template arg_type<I>;