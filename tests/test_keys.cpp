#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"
#include <iostream>
#include <atomic>
#include <map>
#include <string>
#include <ranges>

// Тест: базовая работа KEYS
TEST(KeysTest, BasicKeys) {
    Store store;
    store.set("user:1", std::make_unique<StringValue>("alice"));
    store.set("user:2", std::make_unique<StringValue>("bob"));
    store.set("post:1", std::make_unique<StringValue>("hello"));
    
    // Все ключи
    auto all = store.keys("*");
    EXPECT_EQ(all.size(), 3);
    
    // Ключи с префиксом user:
    auto users = store.keys("user:*");
    EXPECT_EQ(users.size(), 2);
    
    // Точное совпадение
    auto exact = store.keys("user:1");
    EXPECT_EQ(exact.size(), 1);
    EXPECT_EQ(exact[0], "user:1");
}

// Тест: KEYS с ограничением (take)
TEST(KeysTest, KeysWithLimit) {
    Store store;
    store.set("key1", std::make_unique<StringValue>("v1"));
    store.set("key2", std::make_unique<StringValue>("v2"));
    store.set("key3", std::make_unique<StringValue>("v3"));
    store.set("key4", std::make_unique<StringValue>("v4"));
    
    auto limited = store.keys("*", 2);
    EXPECT_EQ(limited.size(), 2);
}

// Тест: ленивость конвейера
// Предикат должен вызваться значительно меньше, чем общее количество элементов
TEST(KeysTest, LazyEvaluation) {
    std::map<std::string, int> test_map;
    for (int i = 0; i < 100; ++i) {
        test_map["key" + std::to_string(i)] = i;
    }
    
    std::atomic<int> predicate_calls{0};
    
    auto view = test_map 
        | std::views::keys
        | std::views::filter([&predicate_calls](const std::string& /*key*/) {
            predicate_calls++;
            return true;  
        })
        | std::views::take(5);

    std::vector<std::string> result;
    for (const auto& key : view) {
        result.push_back(key);
    }
    
    EXPECT_EQ(result.size(), 5);
    

    EXPECT_LT(predicate_calls.load(), 100);
    EXPECT_LE(predicate_calls.load(), 20);  
}

// Тест: пустой результат
TEST(KeysTest, EmptyResult) {
    Store store;
    store.set("user:1", std::make_unique<StringValue>("alice"));
    
    auto result = store.keys("post:*");
    EXPECT_TRUE(result.empty());
}