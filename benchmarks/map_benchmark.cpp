#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include <string>
#include <iomanip>
#include <random>
#include <vector>

template<typename Map>
void benchmark_map(const std::string& name, int n) {
    Map m;
    
    // Генерация ключей
    std::vector<std::string> keys;
    keys.reserve(n);
    for (int i = 0; i < n; ++i) {
        keys.push_back("key_" + std::to_string(i));
    }
    
    // 1. Вставка
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        m[key] = "value_" + key;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double insert_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // 2. Поиск (все ключи)
    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        auto it = m.find(key);
        if (it == m.end()) {
            std::cerr << "Key not found: " << key << "\n";
        }
    }
    end = std::chrono::high_resolution_clock::now();
    double find_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // 3. Обход в порядке ключей
    start = std::chrono::high_resolution_clock::now();
    for (auto it = m.begin(); it != m.end(); ++it) {
        volatile auto val = it->second;
    }
    end = std::chrono::high_resolution_clock::now();
    double iterate_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << std::left << std::setw(20) << name
              << std::fixed << std::setprecision(3)
              << "insert: " << std::setw(10) << insert_time << " ms | "
              << "find: " << std::setw(10) << find_time << " ms | "
              << "iterate: " << iterate_time << " ms\n";
}

// Худший случай для unordered_map: ключи с одинаковым хешем
template<typename Map>
void benchmark_worst_case(const std::string& name, int n) {
    Map m;
    
    // Генерируем ключи, которые дают коллизии
    std::vector<std::string> keys;
    keys.reserve(n);
    for (int i = 0; i < n; ++i) {
        // Ключи, которые могут давать одинаковый хеш
        keys.push_back("collision_" + std::to_string(i * 1000));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        m[key] = "value";
    }
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << std::left << std::setw(20) << name
              << std::fixed << std::setprecision(3)
              << "worst_case_insert: " << time << " ms\n";
}

int main() {
    std::vector<int> sizes = {1000, 100000, 1000000};
    
    for (int n : sizes) {
        std::cout << "\n=== Benchmark with " << n << " elements ===\n";
        benchmark_map<std::map<std::string, std::string>>("map", n);
        benchmark_map<std::unordered_map<std::string, std::string>>("unordered_map", n);
    }
    
    std::cout << "\n=== Worst case (hash collisions) ===\n";
    benchmark_worst_case<std::map<std::string, std::string>>("map", 10000);
    benchmark_worst_case<std::unordered_map<std::string, std::string>>("unordered_map", 10000);
    
    return 0;
}