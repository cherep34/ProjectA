#pragma once

#include <string_view>
#include <variant>
#include <vector>
#include <iostream>

struct SetCommand { std::string_view key; std::string_view value; };
struct GetCommand { std::string_view key; };
struct DelCommand { std::string_view key; };
struct ExistsCommand { std::string_view key; };
struct TypeCommand { std::string_view key; };
struct IncrCommand { std::string_view key; };

struct MsetCommand { 
    std::vector<std::pair<std::string_view, std::string_view>> pairs; 
};

struct KeysCommand { 
    std::string_view pattern; 
};

using Command = std::variant<SetCommand, GetCommand, DelCommand, ExistsCommand, TypeCommand, IncrCommand, MsetCommand, KeysCommand>;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;