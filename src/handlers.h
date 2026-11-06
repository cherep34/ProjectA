#pragma once

#include "store.h"
#include "string_value.h"
#include "int_value.h"
#include "double_value.h"
#include "command_registry.h"
#include "list_value.h"
#include <iostream>
#include <memory>


inline std::unique_ptr<IValue> parse_value(std::string_view sv) {
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


inline void register_all_commands(CommandRegistry& registry, Store& store) {
    
    // SET key value
    registry.register_command("SET", [&](std::string_view key, std::string_view value) {
        store.set(std::string(key), parse_value(value));
        std::cout << "OK\n";
    });
    
    // GET key
    registry.register_command("GET", [&](std::string_view key) {
        if (auto v = store.get(std::string(key)); v) {
            std::cout << (*v)->to_string() << "\n";
        } else {
            std::cout << "(nil)\n";
        }
    });
    
    // DEL key
    registry.register_command("DEL", [&](std::string_view key) {
        std::cout << (store.del(std::string(key)) ? "1" : "0") << "\n";
    });
    
    // EXISTS key
    registry.register_command("EXISTS", [&](std::string_view key) {
        std::cout << (store.get(std::string(key)).has_value() ? "1" : "0") << "\n";
    });
    
    // TYPE key
    registry.register_command("TYPE", [&](std::string_view key) {
        if (auto v = store.get(std::string(key)); v) {
            if ((*v)->is_string()) std::cout << "string\n";
            else if ((*v)->is_int()) std::cout << "int64\n";
            else if ((*v)->is_double()) std::cout << "double\n";
            else if ((*v)->is_list()) std::cout << "list\n";
        } else {
            std::cout << "(nil)\n";
        }
    });
    
    // INCR key
    registry.register_command("INCR", [&](std::string_view key) {
        if (auto v = store.get(std::string(key)); v) {
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
    });
    
    // MSET k1 v1 k2 v2 ... (переменное число аргументов)
    // Для MSET нужен специальный обработчик, так как арность переменная
    registry.register_command("MSET", [&](std::string_view k1, std::string_view v1,
                                          std::string_view k2, std::string_view v2,
                                          std::string_view k3, std::string_view v3,
                                          std::string_view k4, std::string_view v4) {
        store.set(std::string(k1), parse_value(v1));
        store.set(std::string(k2), parse_value(v2));
        store.set(std::string(k3), parse_value(v3));
        store.set(std::string(k4), parse_value(v4));
        std::cout << "OK\n";
    });
    
    // KEYS pattern
    registry.register_command("KEYS", [&](std::string_view pattern) {
        auto keys = store.keys(std::string(pattern));
        if (keys.empty()) {
            std::cout << "(empty list)\n";
        } else {
            for (const auto& key : keys) {
                std::cout << key << "\n";
            }
        }
    });

    // APPEND key value — дописывает value к строковому значению key
    registry.register_command("APPEND", [&](std::string_view key, std::string_view value) {
        auto existing = store.get(std::string(key));
        
        std::string current;
        if (existing.has_value()) {
            if (!(*existing)->is_string()) {
                std::cout << "ERR value is not a string\n";
                return;
            }
            current = dynamic_cast<StringValue*>(*existing)->get();
        }
        
        std::string new_value = current + std::string(value);
        
        store.set(std::string(key), std::make_unique<StringValue>(std::move(new_value)));
        
        std::size_t len = current.size() + value.size();
        std::cout << len << "\n";
    });
    
    // STRLEN key — возвращает длину строкового значения
    registry.register_command("STRLEN", [&](std::string_view key) {
        auto v = store.get(std::string(key));
        if (!v.has_value()) {
            std::cout << "0\n";
            return;
        }
        if (!(*v)->is_string()) {
            std::cout << "ERR value is not a string\n";
            return;
        }
        auto* str_val = dynamic_cast<StringValue*>(*v);
        std::cout << str_val->get().size() << "\n";
    });
    
    // GETSET key value — устанавливает новое значение, возвращает старое
    registry.register_command("GETSET", [&](std::string_view key, std::string_view value) {
        auto old = store.get(std::string(key));
        if (old.has_value()) {
            std::cout << (*old)->to_string() << "\n";
        } else {
            std::cout << "(nil)\n";
        }
        store.set(std::string(key), parse_value(value));
    });

        // LPUSH key value - вставка в начало списка
    registry.register_command("LPUSH", [&](std::string_view key, std::string_view value) {
        auto existing = store.get(std::string(key));
        ListValue* list = nullptr;
        
        if (existing.has_value()) {
            if (!(*existing)->is_list()) {
                std::cout << "ERR key exists but is not a list\n";
                return;
            }
            list = dynamic_cast<ListValue*>(*existing);
        } else {
            auto new_list = std::make_unique<ListValue>();
            list = new_list.get();
            store.set(std::string(key), std::move(new_list));
        }
        
        list->push_front(std::string(value));
        std::cout << list->length() << "\n";
    });
    
    // RPUSH key value - вставка в конец списка
    registry.register_command("RPUSH", [&](std::string_view key, std::string_view value) {
        auto existing = store.get(std::string(key));
        ListValue* list = nullptr;
        
        if (existing.has_value()) {
            if (!(*existing)->is_list()) {
                std::cout << "ERR key exists but is not a list\n";
                return;
            }
            list = dynamic_cast<ListValue*>(*existing);
        } else {
            auto new_list = std::make_unique<ListValue>();
            list = new_list.get();
            store.set(std::string(key), std::move(new_list));
        }
        
        list->push_back(std::string(value));
        std::cout << list->length() << "\n";
    });
    
    // LPOP key - извлечение из начала
    registry.register_command("LPOP", [&](std::string_view key) {
        auto existing = store.get(std::string(key));
        if (!existing.has_value()) {
            std::cout << "(nil)\n";
            return;
        }
        if (!(*existing)->is_list()) {
            std::cout << "ERR key exists but is not a list\n";
            return;
        }
        auto* list = dynamic_cast<ListValue*>(*existing);
        if (list->length() == 0) {
            std::cout << "(nil)\n";
            return;
        }
        std::cout << "\"" << list->pop_front() << "\"\n";
    });
    
    // RPOP key - извлечение из конца
    registry.register_command("RPOP", [&](std::string_view key) {
        auto existing = store.get(std::string(key));
        if (!existing.has_value()) {
            std::cout << "(nil)\n";
            return;
        }
        if (!(*existing)->is_list()) {
            std::cout << "ERR key exists but is not a list\n";
            return;
        }
        auto* list = dynamic_cast<ListValue*>(*existing);
        if (list->length() == 0) {
            std::cout << "(nil)\n";
            return;
        }
        std::cout << "\"" << list->pop_back() << "\"\n";
    });
    
    // LRANGE key start stop - получение диапазона
    registry.register_command("LRANGE", [&](std::string_view key, int start, int stop) {
        auto existing = store.get(std::string(key));
        if (!existing.has_value()) {
            std::cout << "(empty list)\n";
            return;
        }
        if (!(*existing)->is_list()) {
            std::cout << "ERR key exists but is not a list\n";
            return;
        }
        auto* list = dynamic_cast<ListValue*>(*existing);
        auto range = list->range(start, stop);
        if (range.empty()) {
            std::cout << "(empty list)\n";
        } else {
            for (const auto& val : range) {
                std::cout << "\"" << val << "\"\n";
            }
        }
    });

    registry.register_command("KEYSRANGE", [&](std::string_view from, std::string_view to) {
        auto keys = store.keys_range(std::string(from), std::string(to));
        if (keys.empty()) {
            std::cout << "(empty list)\n";
        } else {
            for (const auto& key : keys) {
                std::cout << "\"" << key << "\"\n";
            }
        }
    });

        // COUNTKEYS pattern - подсчёт ключей через std::count_if
    registry.register_command("COUNTKEYS", [&](std::string_view pattern) {
        std::string pat(pattern);
        // Используем std::count_if вместо ручного цикла
        int count = std::count_if(store.begin(), store.end(), [&pat](const std::string& key) {
            return key.find(pat) != std::string::npos;
        });
        std::cout << count << "\n";
    });

    // FINDKEY pattern - поиск первого ключа через std::find_if
    registry.register_command("FINDKEY", [&](std::string_view pattern) {
        std::string pat(pattern);
        // Используем std::find_if вместо ручного цикла
        auto it = std::find_if(store.begin(), store.end(), [&pat](const std::string& key) {
            return key.find(pat) != std::string::npos;
        });
        if (it != store.end()) {
            std::cout << "\"" << *it << "\"\n";
        } else {
            std::cout << "(nil)\n";
        }
    });

    // DEL_KEYS pattern - удаление по условию
    registry.register_command("DEL_KEYS", [&](std::string_view pattern) {
        std::string pat(pattern);
        auto keys = store.keys("*", 0);
        
        // Собираем ключи для удаления (те, что содержат паттерн)
        std::vector<std::string> to_delete;
        std::copy_if(keys.begin(), keys.end(), std::back_inserter(to_delete),
            [&pat](const std::string& key) {
                return key.find(pat) != std::string::npos;
            });
        
        for (const auto& key : to_delete) {
            store.del(key);
        }
        std::cout << to_delete.size() << "\n";
    });

    // SCAN cursor [MATCH pattern] - итерация с курсором
    registry.register_command("SCAN", [&](int cursor, std::string_view pattern) {
        auto all_keys = store.keys(pattern, 0);
        
        if (cursor >= static_cast<int>(all_keys.size())) {
            std::cout << "0\n";
            return;
        }

        int new_cursor = cursor;
        int count = 0;
        
        for (int i = cursor; i < static_cast<int>(all_keys.size()); ++i) {
            std::cout << all_keys[i] << "\n";
            count++;
            if (count >= 10) {
                new_cursor = i + 1;
                break;
            }
            new_cursor = i + 1;
        }
        
        std::cout << new_cursor << "\n";
    });
}