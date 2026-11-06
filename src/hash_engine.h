#pragma once

#include "istorage_engine.h"
#include "store_iterator.h"
#include "fnv_hash.h"
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <stdexcept>

class HashEngine : public IStorageEngine {
private:
    struct Node {
        std::string key;
        std::unique_ptr<IValue> value;
    };

    std::vector<std::list<Node>> buckets;
    std::size_t count_;
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;

    auto find_node(const std::string& key) {
        std::size_t index = fnv1a_hash(key) % buckets.size();
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                return std::make_pair(index, it);
            }
        }
        return std::make_pair(index, bucket.end());
    }

    void rehash() {
        std::size_t new_size = buckets.size() * 2;
        std::vector<std::list<Node>> new_buckets(new_size);
        
        for (auto& bucket : buckets) {
            for (auto& node : bucket) {
                std::size_t new_index = fnv1a_hash(node.key) % new_size;
                new_buckets[new_index].push_back(std::move(node));
            }
        }
        
        buckets = std::move(new_buckets);
    }

public:
    HashEngine() : buckets(16), count_(0) {} // Начальный размер 16 корзин

    void put(const std::string& key, std::unique_ptr<IValue> value) override {
        auto [index, it] = find_node(key);
        if (it != buckets[index].end()) {
            it->value = std::move(value);
        } else {
            buckets[index].push_back({key, std::move(value)});
            count_++;
            
            if (static_cast<double>(count_) / buckets.size() > LOAD_FACTOR_THRESHOLD) {
                rehash();
            }
        }
    }

    IValue* get(const std::string& key) const override {
        std::size_t index = fnv1a_hash(key) % buckets.size();
        const auto& bucket = buckets[index];
        for (const auto& node : bucket) {
            if (node.key == key) {
                return node.value.get();
            }
        }
        return nullptr;
    }

    bool erase(const std::string& key) override {
        std::size_t index = fnv1a_hash(key) % buckets.size();
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                count_--;
                return true;
            }
        }
        return false;
    }

    bool contains(const std::string& key) const override {
        return get(key) != nullptr;
    }

    std::size_t size() const override {
        return count_;
    }

    void clear() override {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        count_ = 0;
    }

    std::vector<std::string> keys_range(const std::string& from, const std::string& to) const override {
        std::vector<std::string> result;
        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                if (node.key >= from && node.key <= to) {
                    result.push_back(node.key);
                }
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    
    StoreIterator begin() const override {

        throw std::runtime_error("HashEngine iterator not fully implemented yet (requires type-erased iterator)");
    }

    StoreIterator end() const override {
        throw std::runtime_error("HashEngine iterator not fully implemented yet");
    }
};