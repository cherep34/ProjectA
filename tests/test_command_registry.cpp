#include <gtest/gtest.h>
#include "command_registry.h"
#include <string>

// Тест: базовая регистрация и вызов
TEST(CommandRegistryTest, RegisterAndExecute) {
    CommandRegistry registry;
    
    std::string result;
    registry.register_command("greet", [&](std::string_view name) {
        result = "Hello, " + std::string(name) + "!";
    });
    
    registry.execute("greet", {"Alice"});
    EXPECT_EQ(result, "Hello, Alice!");
}

// Тест: несколько аргументов разных типов
TEST(CommandRegistryTest, MultipleArgTypes) {
    CommandRegistry registry;
    
    int sum = 0;
    registry.register_command("add", [&](int a, int b) {
        sum = a + b;
    });
    
    registry.execute("add", {"3", "4"});
    EXPECT_EQ(sum, 7);
}

// Тест: неправильное количество аргументов
TEST(CommandRegistryTest, WrongArityThrows) {
    CommandRegistry registry;
    
    registry.register_command("add", [](int a, int b) {
        (void)a; (void)b;
    });
    
    EXPECT_THROW(registry.execute("add", {"1"}), std::runtime_error);
    EXPECT_THROW(registry.execute("add", {"1", "2", "3"}), std::runtime_error);
}

// Тест: неизвестная команда
TEST(CommandRegistryTest, UnknownCommandThrows) {
    CommandRegistry registry;
    EXPECT_THROW(registry.execute("unknown", {}), std::runtime_error);
}

// Тест: has_command
TEST(CommandRegistryTest, HasCommand) {
    CommandRegistry registry;
    
    EXPECT_FALSE(registry.has_command("test"));
    
    registry.register_command("test", []() {});
    
    EXPECT_TRUE(registry.has_command("test"));
    EXPECT_EQ(registry.size(), 1);
}

// Тест: convert для разных типов
TEST(CommandRegistryTest, ConvertTypes) {
    // string_view
    std::string_view sv = convert<std::string_view>("hello");
    EXPECT_EQ(sv, "hello");
    
    // string
    std::string s = convert<std::string>("world");
    EXPECT_EQ(s, "world");
    
    // int
    int i = convert<int>("42");
    EXPECT_EQ(i, 42);
    
    // double
    double d = convert<double>("3.14");
    EXPECT_NEAR(d, 3.14, 0.001);
}

// Тест: convert выбрасывает при невалидном числе
TEST(CommandRegistryTest, ConvertInvalidNumber) {
    EXPECT_THROW(convert<int>("not_a_number"), std::runtime_error);
    EXPECT_THROW(convert<double>("abc"), std::runtime_error);
}