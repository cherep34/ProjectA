#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include "store.h"
#include "std_engine.h"
#include "hash_engine.h"
#include "bad_hash_engine.h"
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

int main(int argc, char* argv[]) {
    // Выбор движка через параметр запуска
    std::unique_ptr<IStorageEngine> engine;
    if (argc > 1 && std::string(argv[1]) == "--engine=hash") {
        std::cout << "Using HashEngine (FNV-1a, chaining)\n";
        engine = std::make_unique<HashEngine>();
    } else if (argc > 1 && std::string(argv[1]) == "--engine=bad") {
        std::cout << "Using BadHashEngine (sum of chars, chaining)\n";
        engine = std::make_unique<BadHashEngine>();
    } else {
        std::cout << "Using StdEngine (std::map)\n";
        engine = std::make_unique<StdEngine>();
    }
    
    Store store(std::move(engine));
    CommandRegistry registry;
    register_all_commands(registry, store);
    
    std::cout << "KVS ready. Type commands:\n"
              << "  Basic: SET, GET, DEL, EXISTS, TYPE, INCR, MSET, KEYS\n"
              << "  String: APPEND, STRLEN, GETSET\n"
              << "  List: LPUSH, RPUSH, LPOP, RPOP, LRANGE\n";
    
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