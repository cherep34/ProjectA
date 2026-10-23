#pragma once

#include <map>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <memory>
#include <ranges>
#include <algorithm>
#include "ivalue.h"
#include "key_like.h"

class Store {
private:
    std::map<std::string, std::unique_ptr<IValue>> store;
    
    void swap(Store& other) noexcept {
        std::swap(store, other.store);
    }
    
    static bool match_pattern(const std::string& key, const std::string& pattern) {
        if (pattern == "*") return true;
        
        if (pattern.size() > 1 && pattern.back() == '*') {
            std::string_view prefix(pattern.data(), pattern.size() - 1);
            return std::string_view(key).starts_with(prefix);
        }
        
        return key == pattern;
    }
    
public:
    template<KeyLike K>
    void set(const K& key, std::unique_ptr<IValue> value) {
        store.insert_or_assign(std::string(to_string_view(key)), std::move(value));
    }
    
    template<KeyLike K>
    std::optional<IValue*> get(const K& key) const {
        if (auto it = store.find(std::string(to_string_view(key))); it != store.end()) {
            return it->second.get();
        }
        return std::nullopt;
    }
    
    template<KeyLike K>
    bool del(const K& key) {
        return store.erase(std::string(to_string_view(key))) > 0;
    }
    
    std::size_t size() const {
        return store.size();
    }
    
    void mset(const std::vector<std::pair<std::string, std::unique_ptr<IValue>>>& pairs);

    // KEYS через ranges-конвейер
    template<KeyLike K>
    std::vector<std::string> keys(const K& pattern, std::size_t limit = 0) const {
        std::string pat(pattern);
        
        if (limit > 0) {
            auto view = store 
                | std::views::keys
                | std::views::filter([&pat](const std::string& key) {
                    return match_pattern(key, pat);
                })
                | std::views::take(limit);
            
            return view | std::ranges::to<std::vector<std::string>>();
        } else {
            auto view = store 
                | std::views::keys
                | std::views::filter([&pat](const std::string& key) {
                    return match_pattern(key, pat);
                });
            
            return view | std::ranges::to<std::vector<std::string>>();
        }
    }
    
    // Сортировка ключей по длине через проекцию
    std::vector<std::string> sorted_keys_by_length() const {
        auto keys_vec = store | std::views::keys | std::ranges::to<std::vector<std::string>>();
        
        std::ranges::sort(keys_vec, {}, &std::string::size);
        
        return keys_vec;
    }

    // Альтернативная версия через лямбду (для сравнения)
    std::vector<std::string> sorted_keys_by_length_lambda() const {
        auto keys_vec = store | std::views::keys | std::ranges::to<std::vector<std::string>>();
        
        std::ranges::sort(keys_vec, [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
        
        return keys_vec;
    }
};