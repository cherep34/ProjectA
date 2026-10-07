#include <gtest/gtest.h>
#include "parser.h"
#include "errors.h"
#include <string>

TEST(ParserExpectedTest, ValidCommands) {
    auto t1 = tokenize("SET k v");
    auto result1 = parse_expected(t1);
    ASSERT_TRUE(result1.has_value());
    EXPECT_TRUE(std::holds_alternative<SetCommand>(result1.value()));

    auto t2 = tokenize("GET k");
    auto result2 = parse_expected(t2);
    ASSERT_TRUE(result2.has_value());
    EXPECT_TRUE(std::holds_alternative<GetCommand>(result2.value()));
    
    auto t3 = tokenize("INCR k");
    auto result3 = parse_expected(t3);
    ASSERT_TRUE(result3.has_value());
    EXPECT_TRUE(std::holds_alternative<IncrCommand>(result3.value()));
}

TEST(ParserExpectedTest, InvalidCommandsReturnError) {
    auto t1 = tokenize("UNKNOWN k");
    auto result1 = parse_expected(t1);
    ASSERT_FALSE(result1.has_value());
    EXPECT_THROW(std::rethrow_exception(std::make_exception_ptr(result1.error())), ParseError);

    auto t2 = tokenize("SET k");
    auto result2 = parse_expected(t2);
    ASSERT_FALSE(result2.has_value());

    auto t3 = tokenize("");
    auto result3 = parse_expected(t3);
    ASSERT_FALSE(result3.has_value());
}

TEST(ParserExpectedTest, MsetCommand) {
    auto t = tokenize("MSET k1 v1 k2 v2");
    auto result = parse_expected(t);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(std::holds_alternative<MsetCommand>(result.value()));
}