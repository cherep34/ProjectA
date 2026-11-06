#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"
#include "store_iterator.h"
#include <algorithm>
#include <vector>

TEST(StoreIteratorTest, ForwardIteratorConcept) {
    static_assert(std::forward_iterator<StoreIterator>, 
        "StoreIterator must satisfy forward_iterator");
    SUCCEED();
}

TEST(StoreIteratorTest, BasicIteration) {
    Store store;
    store.set("key1", std::make_unique<StringValue>("val1"));
    store.set("key2", std::make_unique<StringValue>("val2"));
    store.set("key3", std::make_unique<StringValue>("val3"));
    
    std::vector<std::string> keys;
    for (const auto& key : store) {
        keys.push_back(key);
    }
    
    EXPECT_EQ(keys.size(), 3);
    EXPECT_EQ(keys[0], "key1");
    EXPECT_EQ(keys[1], "key2");
    EXPECT_EQ(keys[2], "key3");
}

TEST(StoreIteratorTest, EmptyStore) {
    Store store;
    
    EXPECT_EQ(store.begin(), store.end());
    
    int count = 0;
    for ([[maybe_unused]] const auto& key : store) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(StoreIteratorTest, STLAlgorithms) {
    Store store;
    store.set("apple", std::make_unique<StringValue>("1"));
    store.set("banana", std::make_unique<StringValue>("2"));
    store.set("cherry", std::make_unique<StringValue>("3"));
    
    int count = std::count_if(store.begin(), store.end(), 
        [](const std::string& key) { return key.size() > 5; });
    EXPECT_EQ(count, 2);  
    
    auto it = std::find_if(store.begin(), store.end(),
        [](const std::string& key) { return key == "banana"; });
    ASSERT_NE(it, store.end());
    EXPECT_EQ(*it, "banana");
}

TEST(StoreIteratorTest, IncrementOperators) {
    Store store;
    store.set("a", std::make_unique<StringValue>("1"));
    store.set("b", std::make_unique<StringValue>("2"));
    
    auto it = store.begin();
    EXPECT_EQ(*it, "a");
    
    ++it;
    EXPECT_EQ(*it, "b");
    
    auto it2 = it++;
    EXPECT_EQ(*it2, "b");
    EXPECT_EQ(it, store.end());
}