#include "store.h"

void Store::mset(const std::vector<std::pair<std::string, std::unique_ptr<IValue>>>& pairs) {
    for (const auto& [key, value] : pairs) {
        engine->put(key, value->clone());
    }
}