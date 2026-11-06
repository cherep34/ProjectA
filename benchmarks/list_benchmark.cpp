#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <string>
#include <iomanip>
#include <fstream>

// Профиль нагрузки: "много вставок в начало и конец, редкое чтение по индексу"
template<typename Container>
void benchmark_container(const std::string& name, int iterations) {
    Container c;
    
    // 1. Вставка в начало (push_front)
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        if constexpr (requires { c.push_front(std::string()); }) {
            c.push_front("key_" + std::to_string(i));
        } else {
            c.insert(c.begin(), "key_" + std::to_string(i));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    double push_front_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // 2. Вставка в конец (push_back)
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        c.push_back("value_" + std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    double push_back_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    // 3. Чтение по индексу (редкое)
    int read_count = iterations / 100;  // 
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < read_count; ++i) {
        if constexpr (requires { c[i]; }) {
            volatile auto val = c[i * 100];  
        } else {
            auto it = c.begin();
            std::advance(it, i * 100);
            volatile auto val = *it;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    double read_time = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << std::left << std::setw(12) << name
              << std::fixed << std::setprecision(3)
              << "push_front: " << std::setw(10) << push_front_time << " ms | "
              << "push_back: " << std::setw(10) << push_back_time << " ms | "
              << "read: " << read_time << " ms\n";
}

int main() {
    const int iterations = 100000;
    
    std::cout << "=== List Container Benchmark ===\n";
    std::cout << "Profile: " << iterations << " insertions at front/back, " 
              << iterations / 100 << " random reads\n\n";
    
    benchmark_container<std::vector<std::string>>("vector", iterations);
    benchmark_container<std::deque<std::string>>("deque", iterations);
    benchmark_container<std::list<std::string>>("list", iterations);
    
    return 0;
}