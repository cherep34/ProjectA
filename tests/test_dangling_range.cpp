#include <gtest/gtest.h>
#include "dangling_range.h"
#include <vector>

// Тест: безопасная версия работает корректно
TEST(DanglingRangeTest, SafeVersionWorks) {
    std::vector<int> data = {1, 2, 6, 7, 3, 8};
    auto result = get_filtered_safe(data);
    
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 6);
    EXPECT_EQ(result[1], 7);
    EXPECT_EQ(result[2], 8);
}

// Тест: dangerous версия компилируется, но итерация по ней — UB
// Мы НЕ итерируем по view, чтобы не падать
TEST(DanglingRangeTest, DangerousVersionCompilesButIsUB) {
    std::vector<int> data = {1, 2, 6, 7, 3, 8};

    [[maybe_unused]] auto view = get_filtered_view_dangerous(data);
    
    SUCCEED() << "Dangerous version compiled successfully. "
              << "Iterating over the returned view is UB (dangling reference). "
              << "In practice, this causes segfault (as demonstrated in separate run).";
}

// Тест: проверка через borrowed_range
TEST(DanglingRangeTest, BorrowedRangeConcept) {
    static_assert(std::ranges::borrowed_range<std::vector<int>&>);

    static_assert(!std::ranges::borrowed_range<std::vector<int>>);
    
    SUCCEED() << "lvalue reference to vector is borrowed_range, prvalue vector is not";
}