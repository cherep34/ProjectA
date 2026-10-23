#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"
#include "int_value.h"
#include "command_registry.h"
#include "handlers.h"
#include <sstream>
#include <iostream>

// Вспомогательный класс для перехвата вывода в cout
class CaptureStdout {
    std::ostringstream buffer;
    std::streambuf* old_cout;
public:
    CaptureStdout() : old_cout(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CaptureStdout() { std::cout.rdbuf(old_cout); }
    std::string str() const { return buffer.str(); }
};

TEST(HandlersTest, AppendToNewKey) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    CaptureStdout cap;
    registry.execute("APPEND", {"mykey", "Hello"});
    EXPECT_EQ(cap.str(), "5\n");
}

TEST(HandlersTest, AppendToExistingKey) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    store.set("mykey", std::make_unique<StringValue>("Hello"));
    
    CaptureStdout cap;
    registry.execute("APPEND", {"mykey", " World"});
    EXPECT_EQ(cap.str(), "11\n");
}

TEST(HandlersTest, AppendToNonString) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    store.set("mykey", std::make_unique<IntValue>(42));
    
    CaptureStdout cap;
    registry.execute("APPEND", {"mykey", " World"});
    EXPECT_EQ(cap.str(), "ERR value is not a string\n");
}

TEST(HandlersTest, StrlenExisting) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    store.set("mykey", std::make_unique<StringValue>("Hello"));
    
    CaptureStdout cap;
    registry.execute("STRLEN", {"mykey"});
    EXPECT_EQ(cap.str(), "5\n");
}

TEST(HandlersTest, StrlenMissing) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    CaptureStdout cap;
    registry.execute("STRLEN", {"missing"});
    EXPECT_EQ(cap.str(), "0\n");
}

TEST(HandlersTest, GetsetWithOldValue) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    store.set("mykey", std::make_unique<StringValue>("old"));
    
    CaptureStdout cap;
    registry.execute("GETSET", {"mykey", "new"});
    EXPECT_EQ(cap.str(), "\"old\"\n");
}

TEST(HandlersTest, GetsetNoOldValue) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    CaptureStdout cap;
    registry.execute("GETSET", {"mykey", "new"});
    EXPECT_EQ(cap.str(), "(nil)\n");
}