#ifndef __MONOLIBS_UTILITY_H
#define __MONOLIBS_UTILITY_H

#include <array>
#include <fmt/base.h>
#include <type_traits>

namespace mono {

template <typename Arg0, typename... Args>
constexpr auto sum(Arg0 arg0, Args... args) -> Arg0 {
    if constexpr (sizeof...(args) == 0)
        return arg0;
    else
        return arg0 + sum(args...);
}

template <typename Arg0, typename... Args>
inline constexpr auto display(Arg0&& arg0, Args&&... args) -> void {
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

} // namespace mono

#endif
