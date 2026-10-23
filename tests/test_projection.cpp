#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"

// Тест: сортировка по длине ключа через проекцию
TEST(ProjectionTest, SortByLengthViaProjection) {
    Store store;
    store.set("a", std::make_unique<StringValue>("1"));
    store.set("abc", std::make_unique<StringValue>("3"));
    store.set("ab", std::make_unique<StringValue>("2"));
    store.set("abcd", std::make_unique<StringValue>("4"));
    
    auto sorted = store.sorted_keys_by_length();
    
    ASSERT_EQ(sorted.size(), 4);
    EXPECT_EQ(sorted[0], "a");      // длина 1
    EXPECT_EQ(sorted[1], "ab");     // длина 2
    EXPECT_EQ(sorted[2], "abc");    // длина 3
    EXPECT_EQ(sorted[3], "abcd");   // длина 4
}

// Тест: сортировка через лямбду даёт тот же результат
TEST(ProjectionTest, SortByLengthViaLambda) {
    Store store;
    store.set("a", std::make_unique<StringValue>("1"));
    store.set("abc", std::make_unique<StringValue>("3"));
    store.set("ab", std::make_unique<StringValue>("2"));
    store.set("abcd", std::make_unique<StringValue>("4"));
    
    auto sorted = store.sorted_keys_by_length_lambda();
    
    ASSERT_EQ(sorted.size(), 4);
    EXPECT_EQ(sorted[0], "a");
    EXPECT_EQ(sorted[1], "ab");
    EXPECT_EQ(sorted[2], "abc");
    EXPECT_EQ(sorted[3], "abcd");
}

// Тест: оба метода дают одинаковый результат
TEST(ProjectionTest, ProjectionAndLambdaGiveSameResult) {
    Store store;
    store.set("x", std::make_unique<StringValue>("v"));
    store.set("long_key", std::make_unique<StringValue>("v"));
    store.set("mid", std::make_unique<StringValue>("v"));
    
    auto by_projection = store.sorted_keys_by_length();
    auto by_lambda = store.sorted_keys_by_length_lambda();
    
    EXPECT_EQ(by_projection, by_lambda);
}