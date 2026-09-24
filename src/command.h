#pragma once

#include <string_view>
#include <variant>
#include <iostream>

struct SetCommand { std::string_view key; std::string_view value; };
struct GetCommand { std::string_view key; };
struct DelCommand { std::string_view key; };
struct ExistsCommand { std::string_view key; };
struct TypeCommand { std::string_view key; };
struct IncrCommand { std::string_view key; };

using Command = std::variant<SetCommand, GetCommand, DelCommand, ExistsCommand, TypeCommand, IncrCommand>;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;