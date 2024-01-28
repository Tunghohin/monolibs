#ifndef __MONOLIBS_UTILITY_H
#define __MONOLIBS_UTILITY_H

#include <array>
#include <chrono>
#include <fmt/base.h>
#include <type_traits>

namespace mono {

template <typename Arg>
constexpr auto sum(Arg arg) -> Arg {
    return arg;
}

template <typename Arg0, typename... Args>
    requires std::is_same_v<Arg0, std::common_type_t<Args...>>
constexpr auto sum(Arg0 arg0, Args... args) -> std::common_type_t<Args...> {
    return arg0 + sum(args...);
}

template <typename Arg0, typename... Args>
constexpr auto display(Arg0&& arg0, Args&&... args) -> void {
    fmt::println("{}", arg0);
    if constexpr (sizeof...(args))
        display(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto accumulate_sum(Args... args) -> void {
    return (0 + ... + args);
}

template <typename... Args>
constexpr auto get_array(Args... args)
    -> std::array<std::common_type_t<Args...>, sizeof...(Args)> {
    using T = std::common_type_t<Args...>;
    return std::array<T, sizeof...(Args)>{args...};
}

template <typename Fn>
auto invoke_and_time(Fn f) -> decltype(f()) {
    auto start_time = std::chrono::steady_clock::now();
    if constexpr (std::is_same_v<decltype(f()), void>) {
        f();
        auto end_time = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration<double, std::milli>(end_time - start_time);
        fmt::println("{}", duration.count());
        return;
    } else {
        auto ret = f();
        auto end_time = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration<double, std::milli>(end_time - start_time);
        fmt::println("{}", duration.count());
        return ret;
    }
}

} // namespace mono

#endif
