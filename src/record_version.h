#pragma once

#include <compare>
#include <string>
#include <cstdint>

// ============================================================
// ВАРИАНТ 1: Автоматическое сравнение через = default
// ============================================================
struct RecordVersion {
    int generation;
    int64_t timestamp;

    // Компилятор сам сгенерирует все 6 операторов сравнения
    auto operator<=>(const RecordVersion&) const = default;
};

struct RecordVersionV2 {
    int generation;
    int64_t timestamp;
    std::string debug_info; // Это поле НЕ должно участвовать в сравнении!

    std::strong_ordering operator<=>(const RecordVersionV2& other) const {
        if (auto cmp = generation <=> other.generation; cmp != 0) {
            return cmp;
        }
        return timestamp <=> other.timestamp;
    }

    bool operator==(const RecordVersionV2& other) const {
        return generation == other.generation && timestamp == other.timestamp;
    }
};