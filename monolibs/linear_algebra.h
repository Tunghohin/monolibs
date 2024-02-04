#ifndef __MONOLIBS_LINEAR_ALGEBRA_H
#define __MONOLIBS_LINEAR_ALGEBRA_H

#include <algorithm>
#include <type_traits>

namespace mono {

template <typename T, int N>
struct generic_vector {
    T pos[N];
    int dimension = N;

    generic_vector();
    template <typename... Args>
        requires std::is_same_v<std::common_type_t<Args...>, T>
    generic_vector(Args...);

    auto operator[](size_t const idx) -> T&;
    auto operator[](size_t const idx) const -> T const&;
};

template <typename T, int N>
generic_vector<T, N>::generic_vector() {
    std::fill(this->pos, this->pos + N, T());
}

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
auto generic_vector<T, N>::operator[](size_t const idx) -> T& {
    return this->pos[idx % this->dimension];
}

template <typename T, int N>
auto generic_vector<T, N>::operator[](size_t const idx) const -> T const& {
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
struct matrix {
    generic_vector<generic_vector<T, M>, N> pos;

    auto operator[](size_t const idx) -> generic_vector<T, M>&;
    auto operator[](size_t const idx) const -> generic_vector<T, M> const&;
};

template <typename T, int N, int M>
auto matrix<T, N, M>::operator[](size_t const idx) -> generic_vector<T, M>& {
    return pos[idx];
}

template <typename T, int N, int M>
auto matrix<T, N, M>::operator[](size_t const idx) const
    -> generic_vector<T, M> const& {
    return pos[idx];
}

} // namespace mono

#endif
