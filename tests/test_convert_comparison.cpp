#include <gtest/gtest.h>
#include "command_registry.h"
#include "convert_enable_if.h"
#include <string>

// Тест: обе версии convert работают одинаково для валидных типов
TEST(ConvertComparisonTest, ValidTypesMatch) {
    EXPECT_EQ(convert<int>("42"), convert_enable_if<int>("42"));
    EXPECT_EQ(convert<double>("3.14"), convert_enable_if<double>("3.14"));
    EXPECT_EQ(convert<std::string>("hello"), convert_enable_if<std::string>("hello"));
}

// Тест: концепция CommandHandler пропускает валидные обработчики
TEST(ConvertComparisonTest, ValidHandlerAccepted) {
    CommandRegistry registry;
    
    // Это должно скомпилироваться, так как int и string поддерживаются
    registry.register_command("test", [](int a, std::string b) {
        (void)a; (void)b;
    });
    
    EXPECT_TRUE(registry.has_command("test"));
}

// Тест: концепция CommandHandler ОТКЛОНЯЕТ обработчик с неподдерживаемым типом
// Этот тест проверяет, что ошибка компиляции короткая и понятная.
// Мы используем макрос, который заведомо не скомпилируется, но в рамках 
// отдельного файла. Здесь мы просто проверяем саму концепцию через static_assert.
TEST(ConvertComparisonTest, InvalidHandlerRejectedByConcept) {
    // Лямбда с неподдерживаемым типом (например, std::vector<int>)
    auto invalid_handler = [](std::vector<int> v) { (void)v; };
    
    // Концепция должна вернуть false
    static_assert(!CommandHandler<decltype(invalid_handler)>, 
        "CommandHandler should reject handlers with unsupported argument types");
    
    SUCCEED() << "Concept correctly rejected invalid handler at compile time.";
}