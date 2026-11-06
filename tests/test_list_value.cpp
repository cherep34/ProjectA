#include <gtest/gtest.h>
#include "store.h"
#include "list_value.h"
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

TEST(ListValueTest, BasicOperations) {
    ListValue list;
    
    list.push_back("a");
    list.push_back("b");
    list.push_front("z");
    
    EXPECT_EQ(list.length(), 3);
    EXPECT_EQ(list.pop_front(), "z");
    EXPECT_EQ(list.pop_back(), "b");
    EXPECT_EQ(list.length(), 1);
}

TEST(ListValueTest, Range) {
    ListValue list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.push_back("d");
    
    auto r1 = list.range(0, 2);
    EXPECT_EQ(r1.size(), 3);
    EXPECT_EQ(r1[0], "a");
    EXPECT_EQ(r1[1], "b");
    EXPECT_EQ(r1[2], "c");
    
    auto r2 = list.range(-2, -1);
    EXPECT_EQ(r2.size(), 2);
    EXPECT_EQ(r2[0], "c");
    EXPECT_EQ(r2[1], "d");
}

TEST(ListValueTest, EmptyList) {
    ListValue list;
    EXPECT_EQ(list.length(), 0);
    EXPECT_THROW(list.pop_front(), std::runtime_error);
    EXPECT_THROW(list.pop_back(), std::runtime_error);
}

TEST(ListCommandsTest, LpushRpush) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    {
        CaptureStdout cap;
        registry.execute("RPUSH", {"mylist", "a"});
        EXPECT_EQ(cap.str(), "1\n");
    }
    {
        CaptureStdout cap;
        registry.execute("RPUSH", {"mylist", "b"});
        EXPECT_EQ(cap.str(), "2\n");
    }
    {
        CaptureStdout cap;
        registry.execute("LPUSH", {"mylist", "z"});
        EXPECT_EQ(cap.str(), "3\n");
    }
}

TEST(ListCommandsTest, LpopRpop) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    registry.execute("RPUSH", {"mylist", "a"});
    registry.execute("RPUSH", {"mylist", "b"});
    registry.execute("RPUSH", {"mylist", "c"});
    
    {
        CaptureStdout cap;
        registry.execute("LPOP", {"mylist"});
        EXPECT_EQ(cap.str(), "\"a\"\n");
    }
    {
        CaptureStdout cap;
        registry.execute("RPOP", {"mylist"});
        EXPECT_EQ(cap.str(), "\"c\"\n");
    }
}

TEST(ListCommandsTest, Lrange) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    registry.execute("RPUSH", {"mylist", "a"});
    registry.execute("RPUSH", {"mylist", "b"});
    registry.execute("RPUSH", {"mylist", "c"});
    registry.execute("RPUSH", {"mylist", "d"});
    
    {
        CaptureStdout cap;
        registry.execute("LRANGE", {"mylist", "0", "2"});
        EXPECT_EQ(cap.str(), "\"a\"\n\"b\"\n\"c\"\n");
    }
    {
        CaptureStdout cap;
        registry.execute("LRANGE", {"mylist", "-2", "-1"});
        EXPECT_EQ(cap.str(), "\"c\"\n\"d\"\n");
    }
}

TEST(ListCommandsTest, PopFromEmpty) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    CaptureStdout cap;
    registry.execute("LPOP", {"nonexistent"});
    EXPECT_EQ(cap.str(), "(nil)\n");
}

TEST(ListCommandsTest, TypeMismatch) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    registry.execute("SET", {"mykey", "value"});
    
    CaptureStdout cap;
    registry.execute("LPUSH", {"mykey", "x"});
    EXPECT_EQ(cap.str(), "ERR key exists but is not a list\n");
}