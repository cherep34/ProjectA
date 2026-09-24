// parser.cpp
#include "parser.h"
#include <cctype>

std::vector<std::string_view> tokenize(std::string_view input) {
    std::vector<std::string_view> tokens;
    size_t i = 0;
    
    while (i < input.size()) {

        if (std::isspace(static_cast<unsigned char>(input[i]))) {
            ++i;
            continue;
        }
        
        if (input[i] == '"') {
            size_t start = ++i;
            while (i < input.size() && input[i] != '"') ++i;
            tokens.emplace_back(input.substr(start, i - start));
            if (i < input.size()) ++i; 
        } 
        else {
            size_t start = i;
            while (i < input.size() && !std::isspace(static_cast<unsigned char>(input[i]))) ++i;
            tokens.emplace_back(input.substr(start, i - start));
        }
    }
    return tokens;
}

std::optional<Command> parse(const std::vector<std::string_view>& tokens) {
    if (tokens.empty()) return std::nullopt;
    
    auto cmd = tokens[0];
    if (cmd == "SET" && tokens.size() == 3) return SetCommand{tokens[1], tokens[2]};
    if (cmd == "GET" && tokens.size() == 2) return GetCommand{tokens[1]};
    if (cmd == "DEL" && tokens.size() == 2) return DelCommand{tokens[1]};
    if (cmd == "EXISTS" && tokens.size() == 2) return ExistsCommand{tokens[1]};
    if (cmd == "TYPE" && tokens.size() == 2) return TypeCommand{tokens[1]};
    if (cmd == "INCR" && tokens.size() == 2) return IncrCommand{tokens[1]};
    
    return std::nullopt;
}