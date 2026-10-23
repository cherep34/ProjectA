#include <gtest/gtest.h>
#include "serializable.h"
#include <fstream>
#include <cstdio>

// Тест: Record удовлетворяет концепции Serializable
TEST(SerializableTest, RecordSatisfiesConcept) {
    static_assert(Serializable<Record>, "Record must be Serializable");
    
    Record record{"test_key", "test_value"};
    auto bytes = record.to_bytes();
    
    EXPECT_FALSE(bytes.empty());
    
    Record restored = Record::from_bytes(bytes);
    EXPECT_EQ(restored.key, "test_key");
    EXPECT_EQ(restored.value, "test_value");
}

// Тест: save_to_file и load_from_file работают с Record
TEST(SerializableTest, SaveAndLoadRecord) {
    const std::string test_file = "test_record.bin";
    std::remove(test_file.c_str());
    
    Record original{"hello", "world"};
    
    save_to_file(original, test_file);
    
    Record loaded = load_from_file<Record>(test_file);
    
    EXPECT_EQ(loaded.key, "hello");
    EXPECT_EQ(loaded.value, "world");
    
    std::remove(test_file.c_str());
}

TEST(SerializableTest, IntDoesNotSatisfyConcept) {
    EXPECT_FALSE(Serializable<int>);
    EXPECT_FALSE(Serializable<double>);
    EXPECT_FALSE(Serializable<std::string>);
}