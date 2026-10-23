#pragma once

#include <vector>
#include <ranges>
#include <string>
#include <iostream>


auto get_filtered_view_dangerous(const std::vector<int>& source) {
    std::vector<int> temp = source;
    
    return temp | std::views::filter([](int x) { return x > 5; });
}

std::vector<int> get_filtered_safe(const std::vector<int>& source) {
    std::vector<int> temp = source;
    return temp | std::views::filter([](int x) { return x > 5; }) 
              | std::ranges::to<std::vector<int>>();
}

void demonstrate_dangling() {
    std::vector<int> data = {1, 2, 6, 7, 3, 8};
    
    auto view = get_filtered_view_dangerous(data);
    
    std::cout << "Attempting to iterate over dangling view...\n";
    std::cout << "Result: ";
    for (int x : view) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}