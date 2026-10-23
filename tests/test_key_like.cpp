#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"
#include "int_value.h"
#include <string_view>

// Тест: концепция KeyLike работает для разных типов
TEST(KeyLikeTest, ConceptSatisfied) {
    static_assert(KeyLike<std::string>, "std::string должен быть KeyLike");
    static_assert(KeyLike<std::string_view>, "std::string_view должен быть KeyLike");
    static_assert(KeyLike<const char*>, "const char* должен быть KeyLike");
    static_assert(!KeyLike<int>, "int НЕ должен быть KeyLike");
    static_assert(!KeyLike<double>, "double НЕ должен быть KeyLike");
}

// Тест: Store::set принимает разные типы ключей
TEST(KeyLikeTest, SetWithDifferentKeyTypes) {
    Store store;
    
    store.set(std::string("key1"), std::make_unique<StringValue>("val1"));
    
    std::string_view sv = "key2";
    store.set(sv, std::make_unique<StringValue>("val2"));
    
    store.set("key3", std::make_unique<StringValue>("val3"));
    
    EXPECT_EQ(store.size(), 3);
}

// Тест: Store::get принимает разные типы ключей
TEST(KeyLikeTest, GetWithDifferentKeyTypes) {
    Store store;
    store.set("key", std::make_unique<StringValue>("value"));

    auto v1 = store.get(std::string("key"));
    auto v2 = store.get(std::string_view("key"));
    auto v3 = store.get("key");
    
    ASSERT_TRUE(v1.has_value());
    ASSERT_TRUE(v2.has_value());
    ASSERT_TRUE(v3.has_value());
    
    EXPECT_EQ((*v1)->to_string(), "\"value\"");
    EXPECT_EQ((*v2)->to_string(), "\"value\"");
    EXPECT_EQ((*v3)->to_string(), "\"value\"");
}

// Тест: Store::del принимает разные типы ключей
TEST(KeyLikeTest, DelWithDifferentKeyTypes) {
    Store store;
    store.set("key", std::make_unique<StringValue>("value"));
    
    EXPECT_TRUE(store.del(std::string_view("key")));
    EXPECT_EQ(store.size(), 0);
}

// Этот код НЕ должен компилироваться:
// store.set(42, std::make_unique<StringValue>("value"));  // Ошибка!
TEST(KeyLikeTest, IntNotAccepted) {
    // Просто проверяем, что концепция возвращает false для int
    EXPECT_FALSE(KeyLike<int>);
}