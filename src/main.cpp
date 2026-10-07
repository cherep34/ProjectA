#include <iostream>
#include <string>
#include "store.h"
#include "parser.h"
#include "command.h"
#include "errors.h"  

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
        },
        [&](const MsetCommand& c) {
            std::vector<std::pair<std::string, Value>> pairs;
            for (const auto& [key, value] : c.pairs) {
                pairs.emplace_back(std::string(key), parse_value(value));
            }
            store.mset(pairs);
            std::cout << "OK\n";
        }
    }, cmd);
}

int main() {
    Store store;
    std::string line;

    std::cout << "KVS ready. Type commands (SET, GET, DEL, EXISTS, TYPE, INCR).\n";
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        try {
            auto tokens = tokenize(line);
            auto cmd = parse(tokens);  
            execute(cmd, store);
        }
        catch (const ParseError& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
        catch (const TypeMismatchError& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
        catch (const KeyNotFoundError& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
        catch (const KvError& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
    }
    return 0;
}