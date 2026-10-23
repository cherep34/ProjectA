#include <gtest/gtest.h>
#include "record_version.h"

// Тест: автоматическое сравнение (RecordVersion)
TEST(RecordVersionTest, DefaultComparison) {
    RecordVersion v1{1, 1000};
    RecordVersion v2{2, 2000};
    RecordVersion v3{1, 1000};

    // Проверяем все 6 операторов
    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v2 > v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_TRUE(v1 == v3);
    EXPECT_TRUE(v1 != v2);
}

// Тест: ручное сравнение игнорирует debug_info (RecordVersionV2)
TEST(RecordVersionTest, ManualComparisonIgnoresField) {
    RecordVersionV2 v1{1, 1000, "debug_A"};
    RecordVersionV2 v2{1, 1000, "debug_B"}; // Отличается только debug_info
    RecordVersionV2 v3{2, 2000, "debug_A"};

    // v1 и v2 должны считаться РАВНЫМИ, несмотря на разный debug_info
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    EXPECT_FALSE(v1 < v2);
    EXPECT_FALSE(v1 > v2);

    // v1 и v3 сравниваются по generation и timestamp
    EXPECT_TRUE(v1 < v3);
    EXPECT_TRUE(v3 > v1);
}