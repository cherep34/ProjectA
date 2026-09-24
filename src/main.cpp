#include <iostream>
#include <string>
#include "store.h"
#include "parser.h"
#include "command.h"

Value parse_value(std::string_view sv) {
    std::string s(sv);
    try {
        size_t pos;
        std::int64_t i = std::stoll(s, &pos);
        if (pos == s.size()) return Value(i);
    } catch(...) {}
    try {
        size_t pos;
        double d = std::stod(s, &pos);
        if (pos == s.size()) return Value(d);
    } catch(...) {}
    return Value(TrackedString(s));
}

void execute(const Command& cmd, Store& store) {
    std::visit(overloaded{
        [&](const SetCommand& c) {
            store.set(std::string(c.key), parse_value(c.value));
            std::cout << "OK\n";
        },
        [&](const GetCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                std::visit(overloaded{
                    [](const TrackedString& s) { std::cout << "\"" << s.to_string() << "\"\n"; },
                    [](auto arg) { std::cout << arg << "\n"; }
                }, *v);
            } else {
                std::cout << "(nil)\n";
            }
        },
        [&](const DelCommand& c) {
            std::cout << (store.del(std::string(c.key)) ? "1" : "0") << "\n";
        },
        [&](const ExistsCommand& c) {
            std::cout << (store.get(std::string(c.key)).has_value() ? "1" : "0") << "\n";
        },
        [&](const TypeCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                std::visit(overloaded{
                    [](const TrackedString&) { std::cout << "string\n"; },
                    [](std::int64_t) { std::cout << "int64\n"; },
                    [](double) { std::cout << "double\n"; }
                }, *v);
            } else {
                std::cout << "(nil)\n";
            }
        },
        [&](const IncrCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                if (auto* int_val = std::get_if<std::int64_t>(&*v)) {
                    (*int_val)++;
                    store.set(std::string(c.key), *v);
                    std::cout << *int_val << "\n";
                } else {
                    std::cout << "ERR value is not an integer\n";
                }
            } else {
                std::cout << "ERR no such key\n";
            }
        }
    }, cmd);
}

int main() {
    Store store;
    std::string line;

    std::cout << "KVS ready. Type commands (SET, GET, DEL, EXISTS, TYPE, INCR).\n";
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        auto tokens = tokenize(line);
        if (auto cmd_opt = parse(tokens); cmd_opt) {
            execute(*cmd_opt, store);
        } else {
            std::cout << "ERR unknown command or wrong number of arguments\n";
        }
    }
    return 0;
}