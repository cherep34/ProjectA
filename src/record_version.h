#pragma once

#include <compare>
#include <string>
#include <cstdint>


struct RecordVersion {
    int generation;
    int64_t timestamp;

    auto operator<=>(const RecordVersion&) const = default;
};

struct RecordVersionV2 {
    int generation;
    int64_t timestamp;
    std::string debug_info;

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