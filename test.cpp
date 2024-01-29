#include <chrono>
#include <fmt/base.h>
#include <functional.h>
#include <thread>
#include <utility.h>

template <int N>
struct fibonacci {
    static constexpr int value =
        fibonacci<N - 1>::value + fibonacci<N - 2>::value;
};

template <>
struct fibonacci<1> {
    static constexpr int value = 1;
};

template <>
struct fibonacci<2> {
    static constexpr int value = 1;
};

auto main() -> int {
    constexpr int res = fibonacci<10>::value;
    fmt::println("{}", mono::cppdemangle<decltype(res)>());
    return 0;
}
