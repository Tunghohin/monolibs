#ifndef __MONOLIBS_UTILITY_H
#define __MONOLIBS_UTILITY_H

#include <array>

namespace mono {

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
