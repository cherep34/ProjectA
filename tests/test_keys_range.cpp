#include <gtest/gtest.h>
#include "store.h"
#include "string_value.h"
#include "command_registry.h"
#include "handlers.h"
#include <sstream>

class CaptureStdout {
    std::ostringstream buffer;
    std::streambuf* old_cout;
public:
    CaptureStdout() : old_cout(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CaptureStdout() { std::cout.rdbuf(old_cout); }
    std::string str() const { return buffer.str(); }
};

TEST(KeysRangeTest, BasicRange) {
    Store store;
    store.set("apple", std::make_unique<StringValue>("1"));
    store.set("banana", std::make_unique<StringValue>("2"));
    store.set("cherry", std::make_unique<StringValue>("3"));
    store.set("date", std::make_unique<StringValue>("4"));
    store.set("elderberry", std::make_unique<StringValue>("5"));
    
    auto range = store.keys_range("banana", "date");
    ASSERT_EQ(range.size(), 3);
    EXPECT_EQ(range[0], "banana");
    EXPECT_EQ(range[1], "cherry");
    EXPECT_EQ(range[2], "date");
}

TEST(KeysRangeTest, EmptyRange) {
    Store store;
    store.set("a", std::make_unique<StringValue>("1"));
    store.set("z", std::make_unique<StringValue>("2"));
    
    auto range = store.keys_range("b", "y");
    EXPECT_TRUE(range.empty());
}

TEST(KeysRangeTest, CommandIntegration) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    registry.execute("SET", {"apple", "1"});
    registry.execute("SET", {"banana", "2"});
    registry.execute("SET", {"cherry", "3"});
    
    {
        CaptureStdout cap;
        registry.execute("KEYSRANGE", {"apple", "banana"});
        EXPECT_EQ(cap.str(), "\"apple\"\n\"banana\"\n");
    }
}