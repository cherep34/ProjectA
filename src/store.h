#pragma once

#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <memory>
#include <ranges>
#include <algorithm>
#include "ivalue.h"
#include "store_iterator.h"
#include "key_like.h"
#include "istorage_engine.h"
#include "std_engine.h"

class Store {
private:
    std::unique_ptr<IStorageEngine> engine;
    
    void swap(Store& other) noexcept {
        std::swap(engine, other.engine);
    }
    
    static bool match_pattern(const std::string& key, const std::string& pattern) {
        if (pattern == "*") return true;
        if (pattern.size() > 1 && pattern.back() == '*') {
            std::string_view prefix(pattern.data(), pattern.size() - 1);
            return std::string_view(key).starts_with(prefix);
        }
        return key == pattern;
    }
    
    // Вспомогательный метод: собрать все ключи в вектор
    std::vector<std::string> all_keys() const {
        std::vector<std::string> keys;
        for (auto it = engine->begin(); it != engine->end(); ++it) {
            keys.push_back(*it);
        }
        return keys;
    }
    
public:
    Store() : engine(std::make_unique<StdEngine>()) {}
    explicit Store(std::unique_ptr<IStorageEngine> eng) : engine(std::move(eng)) {}
    
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;
    Store(Store&& other) noexcept = default;
    Store& operator=(Store&& other) noexcept = default;
    
    StoreIterator begin() const { return engine->begin(); }
    StoreIterator end() const { return engine->end(); }
    
    template<KeyLike K>
    void set(const K& key, std::unique_ptr<IValue> value) {
        engine->put(std::string(to_string_view(key)), std::move(value));
    }
    
    template<KeyLike K>
    std::optional<IValue*> get(const K& key) const {
        IValue* ptr = engine->get(std::string(to_string_view(key)));
        if (ptr) return ptr;
        return std::nullopt;
    }
    
    template<KeyLike K>
    bool del(const K& key) {
        return engine->erase(std::string(to_string_view(key)));
    }
    
    std::size_t size() const { return engine->size(); }
    
    void mset(const std::vector<std::pair<std::string, std::unique_ptr<IValue>>>& pairs);
    
    template<KeyLike K>
    std::vector<std::string> keys(const K& pattern, std::size_t limit = 0) const {
        std::string pat(pattern);
        auto all = all_keys();
        
        auto view = all | std::views::filter([&pat](const std::string& key) {
            return match_pattern(key, pat);
        });
        
        if (limit > 0) {
            return (view | std::views::take(limit)) | std::ranges::to<std::vector<std::string>>();
        }
        return view | std::ranges::to<std::vector<std::string>>();
    }
    
    std::vector<std::string> sorted_keys_by_length() const {
        auto keys_vec = all_keys();
        std::ranges::sort(keys_vec, {}, &std::string::size);
        return keys_vec;
    }
    
    std::vector<std::string> sorted_keys_by_length_lambda() const {
        auto keys_vec = all_keys();
        std::ranges::sort(keys_vec, [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
        return keys_vec;
    }

    std::vector<std::string> keys_range(const std::string& from, const std::string& to) const {
        return engine->keys_range(from, to);
    }
};