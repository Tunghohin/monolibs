#ifndef __MONOLIBS_UTILITY_H
#define __MONOLIBS_UTILITY_H

#include <array>
#include <atomic>
#include <chrono>
#include <fmt/format.h>
#include <functional>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>
#if (defined(__GNUC__) || defined(__clang__)) && __has_include(<cxxabi.h>)
#include <cstdlib>
#include <cxxabi.h>
#endif

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
    fmt::print("{}\n", arg0);
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
        fmt::print("{}\n", duration.count());
        return;
    } else {
        auto ret = f();
        auto end_time = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration<double, std::milli>(end_time - start_time);
        fmt::print("{}\n", duration.count());
        return ret;
    }
}

// compile time fibonacci sequence
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

// demangle type type
static std::string cppdemangle(const char* name) {
#if (defined(__GNUC__) || defined(__clang__)) && __has_include(<cxxabi.h>)
    int status;
    char* p = abi::__cxa_demangle(name, 0, 0, &status);
    std::string s = p ? p : name;
    std::free(p);
#else
    std::string s = name;
#endif
    return s;
}

static std::string cppdemangle(std::type_info const& type) {
    return cppdemangle(type.name());
}

template <class T>
static std::string cppdemangle() {
    std::string s{
        cppdemangle(typeid(std::remove_cv_t<std::remove_reference_t<T>>))};
    if (std::is_const_v<std::remove_reference_t<T>>)
        s += " const";
    if (std::is_volatile_v<std::remove_reference_t<T>>)
        s += " volatile";
    if (std::is_lvalue_reference_v<T>)
        s += " &";
    if (std::is_rvalue_reference_v<T>)
        s += " &&";
    return s;
}

template <typename T>
auto lrvalue_test(T&& obj) -> void {
    if constexpr (std::is_rvalue_reference_v<decltype(std::forward<T>(obj))>) {
        fmt::print("is rvalue\n");
    } else {
        fmt::print("is lvalue\n");
    }
}

class spin_lock {
public:
    auto lock() -> void {
        while (flag_.test_and_set(std::memory_order_acquire)) {
        };
    }
    auto unlock() -> void { flag_.clear(std::memory_order_release); }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

template <typename T, T... ints>
void print_index_seq(std::integer_sequence<T, ints...> int_seq) {
    fmt::println("{}", int_seq.size());
    ((fmt::print("{} ", ints)), ...);
    fmt::println("");
}

template <typename Fn, typename Tup, std::size_t... Idx>
constexpr auto apply_impl(Fn&& f, Tup&& t, std::index_sequence<Idx...>) {
    return std::invoke(std::forward<Fn>(f),
                       std::get<Idx>(std::forward<Tup>)...);
}

template <typename Fn, typename Tup>
constexpr auto apply(Fn&& f, Tup&& t) {
    return apply_impl(std::forward<Fn>(f), std::forward<Tup>(t),
                      std::make_index_sequence<
                          std::tuple_size_v<std::remove_reference_t<Tup>>>{});
}

} // namespace mono

#endif
