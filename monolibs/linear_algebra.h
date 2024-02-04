#ifndef __MONOLIBS_LINEAR_ALGEBRA_H
#define __MONOLIBS_LINEAR_ALGEBRA_H

#include <algorithm>
#include <type_traits>

namespace mono {

template <typename T, int N>
struct generic_vector {
    T pos[N];
    int dimension = N;

    template <typename... Args>
        requires std::is_same_v<std::common_type_t<Args...>, T>
    generic_vector(Args...);

    constexpr auto operator[](size_t const idx) const -> T;
};

template <typename T, int N>
template <typename... Args>
    requires std::is_same_v<std::common_type_t<Args...>, T>
generic_vector<T, N>::generic_vector(Args... args) {
    static_assert(sizeof...(Args) == N,
                  "Number of arguments must match vector dimension");
    size_t i = 0;
    ((pos[i++] = args), ...);
}

template <typename T, int N>
constexpr auto generic_vector<T, N>::operator[](size_t const idx) const -> T {
    return this->pos[idx % this->dimension];
}

template <typename T>
struct vector2 : public generic_vector<T, 2> {
    template <typename... Args>
        requires std::is_same_v<std::common_type_t<Args...>, T>
    vector2(Args... args)
        : generic_vector<T, 2>(args...), x(this->pos[0]), y(this->pos[1]) {}

    T& x;
    T& y;
};

template <typename T>
struct vector3 : public generic_vector<T, 3> {
    template <typename... Args>
        requires std::is_same_v<std::common_type_t<Args...>, T>
    vector3(Args... args)
        : generic_vector<T, 3>(args...), x(this->pos[0]), y(this->pos[1]),
          z(this->pos[2]) {}

    T& x;
    T& y;
    T& z;
};

template <typename T, int N, int M>
struct matric {
    generic_vector<T, M> pos[N];
};

} // namespace mono

#endif
