#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "store.h"
#include "command_registry.h"
#include "handlers.h"

std::vector<std::string> tokenize_simple(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    Store store;
    CommandRegistry registry;
    
    register_all_commands(registry, store);
    
    std::cout << "KVS ready. Type commands (SET, GET, DEL, EXISTS, TYPE, INCR, MSET, KEYS, APPEND, STRLEN, GETSET).\n";
    
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        try {
            auto tokens = tokenize_simple(line);
            if (tokens.empty()) continue;
            
            std::string cmd = tokens[0];
            std::vector<std::string> args(tokens.begin() + 1, tokens.end());
            
            registry.execute(cmd, args);
        }
        catch (const std::exception& e) {
            std::cout << "ERR " << e.what() << "\n";
        }
    }
    return 0;
}