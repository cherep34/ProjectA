#include <gtest/gtest.h>
#include "store.h"
#include "value.h"
#include <stdexcept>

TEST(MsetTest, AtomicSuccess) {
    Store store;
    store.set("existing", Value(TrackedString("old")));
    
    std::vector<std::pair<std::string, Value>> pairs = {
        {"k1", Value(TrackedString("v1"))},
        {"k2", Value(TrackedString("v2"))}
    };
    
    store.mset(pairs);
    
    EXPECT_TRUE(store.get("k1").has_value());
    EXPECT_TRUE(store.get("k2").has_value());
    EXPECT_TRUE(store.get("existing").has_value());
}

TEST(MsetTest, AtomicRollbackOnException) {
    Store store;
    store.set("existing", Value(TrackedString("old")));
    
    TrackedString::reset();
    TrackedString::throw_on_Nth_copy = 3; 
    
    std::vector<std::pair<std::string, Value>> pairs = {
        {"k1", Value(TrackedString("v1"))},
        {"k2", Value(TrackedString("v2"))},
        {"k3", Value(TrackedString("v3"))}
    };
    
    EXPECT_THROW(store.mset(pairs), std::runtime_error);
    
    EXPECT_FALSE(store.get("k1").has_value());
    EXPECT_FALSE(store.get("k2").has_value());
    EXPECT_FALSE(store.get("k3").has_value());
    
    auto existing = store.get("existing");
    ASSERT_TRUE(existing.has_value());
    EXPECT_EQ(std::get<TrackedString>(*existing).to_string(), "old");
    
    TrackedString::reset();
}