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
            std::cout << "OK" << std::endl;
        },
        [&](const GetCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                std::visit(overloaded{
                    [](const TrackedString& s) { std::cout << "\"" << s.to_string() << "\"" << std::endl; },
                    [](auto arg) { std::cout << arg << std::endl; }
                }, *v);
            } else {
                std::cout << "(nil)" << std::endl;
            }
        },
        [&](const DelCommand& c) {
            std::cout << (store.del(std::string(c.key)) ? "1" : "0") << std::endl;
        },
        [&](const ExistsCommand& c) {
            std::cout << (store.get(std::string(c.key)).has_value() ? "1" : "0") << std::endl;
        },
        [&](const TypeCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                std::visit(overloaded{
                    [](const TrackedString&) { std::cout << "string" << std::endl; },
                    [](std::int64_t) { std::cout << "int64" << std::endl; },
                    [](double) { std::cout << "double" << std::endl; }
                }, *v);
            } else {
                std::cout << "(nil)" << std::endl;
            }
        },
        [&](const IncrCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                if (auto* int_val = std::get_if<std::int64_t>(&*v)) {
                    (*int_val)++;
                    store.set(std::string(c.key), *v);
                    std::cout << *int_val << std::endl;
                } else {
                    std::cout << "not integer" << std::endl;
                }
            } else {
                std::cout << "no key" << std::endl;
            }
        }
    }, cmd);
}

int main() {
    Store store;
    std::string line;

    std::cout << "KVS: commands - SET, GET, DEL, EXISTS, TYPE, INCR" << std::endl;
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        auto tokens = tokenize(line);
        if (auto cmd_opt = parse(tokens); cmd_opt) {
            execute(*cmd_opt, store);
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }
    return 0;
}