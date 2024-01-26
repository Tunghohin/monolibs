#ifndef __MONOLIBS_FUNCTIONAL_H
#define __MONOLIBS_FUNCTIONAL_H

#include <type_traits>

namespace mono {

template<typename T>
requires std::is_integral_v<T>
[[nodiscard]] constexpr auto func(T const args) noexcept -> T {
    return args * args;
}


} // namespace mono

#endif
