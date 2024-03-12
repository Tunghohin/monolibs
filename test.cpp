#include <array>
#include <iostream>
#include <memory_resource>
#include <numeric>

#include <design_pattern.h>

template <typename T>
auto display(T& container) -> void {
    for (auto&& item : container) {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
}

auto main() -> int {
    std::array<int, 5> arr;
    std::iota(arr.begin(), arr.end(), 0);
    display(arr);
    auto v = std::accumulate(arr.begin(), arr.end(), 0);
    display(arr);
    std::cout << v << std::endl;
    return 0;
}
