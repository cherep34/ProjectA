#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include "fnv_hash.h"
#include "bad_hash.h"

int main() {
    const int N = 100000;
    
    // Генерируем ключи
    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back("key_" + std::to_string(i));
    }
    
    // Замер FNV-1a
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        volatile auto h = fnv1a_hash(key);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double fnv_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // Замер bad_hash
    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        volatile auto h = bad_hash(key);
    }
    end = std::chrono::high_resolution_clock::now();
    double bad_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << "Hash function benchmark (" << N << " keys):\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "FNV-1a:   " << fnv_time << " ms\n";
    std::cout << "bad_hash: " << bad_time << " ms\n";
    
    // Демонстрация коллизий
    std::cout << "\nCollision demo:\n";
    std::cout << "bad_hash(\"abc\") = " << bad_hash("abc") << "\n";
    std::cout << "bad_hash(\"bca\") = " << bad_hash("bca") << "\n";
    std::cout << "bad_hash(\"cab\") = " << bad_hash("cab") << "\n";
    std::cout << "(Все три строки дают одинаковый хеш!)\n";
    
    std::cout << "\nfnv1a_hash(\"abc\") = " << fnv1a_hash("abc") << "\n";
    std::cout << "fnv1a_hash(\"bca\") = " << fnv1a_hash("bca") << "\n";
    std::cout << "fnv1a_hash(\"cab\") = " << fnv1a_hash("cab") << "\n";
    std::cout << "(Все три строки дают РАЗНЫЕ хеши!)\n";
    
    return 0;
}