#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include "istorage_engine.h"
#include "std_engine.h"
#include "hash_engine.h"
#include "bad_hash_engine.h"
#include "string_value.h"

template<typename EngineType>
void benchmark_engine(const std::string& name, int n) {
    EngineType engine;
    
    // 1. Вставка
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        engine.put("key_" + std::to_string(i), std::make_unique<StringValue>("value_" + std::to_string(i)));
    }
    auto end = std::chrono::high_resolution_clock::now();
    double insert_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // 2. Поиск
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        engine.get("key_" + std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    double find_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << std::left << std::setw(15) << name
              << std::fixed << std::setprecision(3)
              << "insert: " << std::setw(10) << insert_time << " ms | "
              << "find: " << find_time << " ms\n";
}

int main() {
    std::vector<int> sizes = {1000, 100000, 1000000};
    
    for (int n : sizes) {
        std::cout << "\n=== Benchmark with " << n << " elements ===\n";
        benchmark_engine<StdEngine>("StdEngine", n);
        benchmark_engine<HashEngine>("HashEngine", n);
        benchmark_engine<BadHashEngine>("BadHashEngine", n);
    }
    
    return 0;
}