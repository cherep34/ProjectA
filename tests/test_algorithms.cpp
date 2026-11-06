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

TEST(AlgorithmsTest, CountKeys) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);

    registry.execute("SET", {"user:1", "a"});
    registry.execute("SET", {"user:2", "b"});
    registry.execute("SET", {"post:1", "c"});

    CaptureStdout cap;
    registry.execute("COUNTKEYS", {"user"});
    EXPECT_EQ(cap.str(), "2\n");
}

TEST(AlgorithmsTest, FindKey) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);

    registry.execute("SET", {"apple", "1"});
    registry.execute("SET", {"banana", "2"});

    {
        CaptureStdout cap;
        registry.execute("FINDKEY", {"ban"});
        EXPECT_EQ(cap.str(), "\"banana\"\n");
    }
    {
        CaptureStdout cap;
        registry.execute("FINDKEY", {"xyz"});
        EXPECT_EQ(cap.str(), "(nil)\n");
    }
}

TEST(AlgorithmsTest, DelKeys) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);

    registry.execute("SET", {"temp:1", "a"});
    registry.execute("SET", {"temp:2", "b"});
    registry.execute("SET", {"keep:1", "c"});

    {
        CaptureStdout cap;
        registry.execute("DEL_KEYS", {"temp"});
        EXPECT_EQ(cap.str(), "2\n");
    }
    {
        CaptureStdout cap;
        registry.execute("COUNTKEYS", {"temp"});
        EXPECT_EQ(cap.str(), "0\n");
    }
}

TEST(AlgorithmsTest, ScanCommand) {
    Store store;
    CommandRegistry registry;
    register_all_commands(registry, store);

    for (int i = 0; i < 5; ++i) {
        registry.execute("SET", {"key_" + std::to_string(i), "val"});
    }

    CaptureStdout cap;
    registry.execute("SCAN", {"0", "*"});
    EXPECT_TRUE(cap.str().find("key_0") != std::string::npos);
}