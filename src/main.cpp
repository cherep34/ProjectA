#include <iostream>
#include <string>
#include "store.h"
#include "parser.h"
#include "command.h"
#include "errors.h"
#include "string_value.h"
#include "int_value.h"
#include "double_value.h"

std::unique_ptr<IValue> parse_value(std::string_view sv) {
    std::string s(sv);
    try {
        size_t pos;
        std::int64_t i = std::stoll(s, &pos);
        if (pos == s.size()) return std::make_unique<IntValue>(i);
    } catch(...) {}
    try {
        size_t pos;
        double d = std::stod(s, &pos);
        if (pos == s.size()) return std::make_unique<DoubleValue>(d);
    } catch(...) {}
    return std::make_unique<StringValue>(s);
}

void execute(const Command& cmd, Store& store) {
    std::visit(overloaded{
        [&](const SetCommand& c) {
            store.set(std::string(c.key), parse_value(c.value));
            std::cout << "OK\n";
        },
        [&](const GetCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                std::cout << (*v)->to_string() << "\n";
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
                if ((*v)->is_string()) std::cout << "string\n";
                else if ((*v)->is_int()) std::cout << "int64\n";
                else if ((*v)->is_double()) std::cout << "double\n";
                else if ((*v)->is_list()) std::cout << "list\n";
            } else {
                std::cout << "(nil)\n";
            }
        },
        [&](const IncrCommand& c) {
            if (auto v = store.get(std::string(c.key)); v) {
                if ((*v)->is_int()) {
                    auto* int_val = dynamic_cast<IntValue*>(*v);
                    int_val->set(int_val->get() + 1);
                    std::cout << int_val->get() << "\n";
                } else {
                    std::cout << "ERR value is not an integer\n";
                }
            } else {
                std::cout << "ERR no such key\n";
            }
        },
        [&](const KeysCommand& c) {
            auto keys = store.keys(std::string(c.pattern));
            if (keys.empty()) {
                std::cout << "(empty list)\n";
            } else {
                for (const auto& key : keys) {
                    std::cout << key << "\n";
                }
            }
        },
        [&](const MsetCommand& c) {
            std::vector<std::pair<std::string, std::unique_ptr<IValue>>> pairs;
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

    std::cout << "KVS ready. Type commands (SET, GET, DEL, EXISTS, TYPE, INCR, MSET).\n";
    
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