#pragma once

#include <compare>
#include <string>
#include <cstdint>


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

        // Сортировка ключей по длине через проекцию
    std::vector<std::string> sorted_keys_by_length() const {
        auto keys_vec = store | std::views::keys | std::ranges::to<std::vector<std::string>>();
        
        std::ranges::sort(keys_vec, {}, &std::string::size);
        
        return keys_vec;
    }

    // Альтернативная версия через лямбду (для сравнения)
    std::vector<std::string> sorted_keys_by_length_lambda() const {
        auto keys_vec = store | std::views::keys | std::ranges::to<std::vector<std::string>>();
        
        std::ranges::sort(keys_vec, [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
        
        return keys_vec;
    }
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