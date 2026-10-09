#pragma once

#include <string>
#include <memory>
#include <map>
#include "command.h"

class CommandCache {
private:
    std::map<std::string, std::weak_ptr<Command>> cache_;

public:
    void add(const std::string& text, std::shared_ptr<Command> cmd) {
        cache_[text] = cmd;
    }


    std::shared_ptr<Command> get(const std::string& text) {
        auto it = cache_.find(text);
        if (it != cache_.end()) {
            return it->second.lock(); 
        }
        return nullptr;
    }


    void cleanup() {
        for (auto it = cache_.begin(); it != cache_.end(); ) {
            if (it->second.expired()) {
                it = cache_.erase(it); 
            } else {
                ++it;
            }
        }
    }

    size_t size() const { 
        return cache_.size(); 
    }
};