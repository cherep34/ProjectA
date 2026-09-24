#include <gtest/gtest.h>
#include "owning_buffer.h"
#include <cstring>

TEST(OwningBufferTest, CopyCreatesIndependentMemory) {
    OwningBuffer buf1(10);
    std::strcpy(buf1.get_data(), "original");
    
    OwningBuffer buf2 = buf1;
    buf2.get_data()[0] = 'X';
    
    EXPECT_STREQ(buf1.get_data(), "original");
    EXPECT_STREQ(buf2.get_data(), "Xriginal");
}

TEST(OwningBufferTest, SelfAssignmentIsSafe) {
    OwningBuffer buf(5);
    std::strcpy(buf.get_data(), "hello");
    
    buf = buf; 
    
    EXPECT_EQ(buf.get_size(), 5);
    EXPECT_STREQ(buf.get_data(), "hello");
}

TEST(OwningBufferTest, MovedFromObjectIsValid) {
    OwningBuffer buf1(5);
    std::strcpy(buf1.get_data(), "hello");
    
    OwningBuffer buf2 = std::move(buf1);
    
    EXPECT_EQ(buf1.get_size(), 0);
    EXPECT_EQ(buf1.get_data(), nullptr);
    
    EXPECT_EQ(buf2.get_size(), 5);
    EXPECT_STREQ(buf2.get_data(), "hello");
}