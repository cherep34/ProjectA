#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include "value.h"
#include "store.h"

TEST(ValueTest, MoveDoesNotCopy) {
    Value v1(TrackedString("test"));
    ValueTracking::reset_counters(); 
    
    Value v2 = std::move(v1);
    
    EXPECT_EQ(ValueTracking::get_move_ctor_count(), 1);
    EXPECT_EQ(ValueTracking::get_copy_ctor_count(), 0);
}

TEST(ValueTest, InsertTemporaryDoesNotCopy) {
    Store store;
    ValueTracking::reset_counters();
    
    store.set("key", Value(TrackedString("temp_value")));
    
    EXPECT_EQ(ValueTracking::get_copy_ctor_count(), 0);
}