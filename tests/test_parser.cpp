#include <gtest/gtest.h>
#include "parser.h"
#include <string>

TEST(LexerTest, BasicTokenization) {
    std::string input = "SET key value";
    auto tokens = tokenize(input);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "SET");
    EXPECT_EQ(tokens[1], "key");
    EXPECT_EQ(tokens[2], "value");
}

TEST(LexerTest, QuotesHandling) {
    std::string input = "SET greeting \"hello world\"";
    auto tokens = tokenize(input);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[2], "hello world");
}

TEST(LexerTest, EmptyAndSpaces) {
    EXPECT_TRUE(tokenize("").empty());
    EXPECT_TRUE(tokenize("   ").empty());
}

TEST(ParserTest, ValidCommands) {
    auto t1 = tokenize("SET k v");
    EXPECT_TRUE(std::holds_alternative<SetCommand>(*parse(t1)));

    auto t2 = tokenize("GET k");
    EXPECT_TRUE(std::holds_alternative<GetCommand>(*parse(t2)));
    
    auto t3 = tokenize("INCR k");
    EXPECT_TRUE(std::holds_alternative<IncrCommand>(*parse(t3)));
}

TEST(ParserTest, InvalidCommands) {
    EXPECT_FALSE(parse(tokenize("UNKNOWN k")).has_value());
    EXPECT_FALSE(parse(tokenize("SET k")).has_value()); 
    EXPECT_FALSE(parse(tokenize("")).has_value());
}

TEST(ParserTest, DanglingStringViewExplanation) {

    std::string safe_buffer = "SET k v";
    auto tokens = tokenize(safe_buffer);
    auto cmd = *parse(tokens);
    
    auto set_cmd = std::get<SetCommand>(cmd);
    EXPECT_EQ(set_cmd.key, "k");
    EXPECT_EQ(set_cmd.value, "v");
}