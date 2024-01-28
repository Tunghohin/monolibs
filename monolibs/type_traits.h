#ifndef __MONOLIBS_TYPE_TRAITS_H
#define __MONOLIBS_TYPE_TRAITS_H

namespace mono {

template <typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant<T, v>;

    constexpr operator value_type() const noexcept { return value; }
    constexpr auto operator()() const noexcept -> value_type { return value; }
};

template <typename T, T v>
constexpr T integral_constant<T, v>::value;

using true_type = integral_constant<bool, true>;

using false_type = integral_constant<bool, false>;

template <bool v>
using bool_constant = integral_constant<bool, v>;

template <typename>
struct is_void : false_type {};

template <>
struct is_void<void> : true_type {};

template <typename T>
constexpr bool is_void_v = is_void<T>::value;

} // namespace mono

#endif
