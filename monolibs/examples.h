#include <algorithm>
#include <fmt/base.h>
#include <utility>

// class with ptr and move ctor
class memory_pool {
public:
    static int constexpr pool_size = 4096;

    memory_pool() : m_pdata(new char[pool_size]) { fmt::println("ctor"); }

    memory_pool(memory_pool const& obj) : m_pdata(new char[pool_size]) {
        std::copy(obj.m_pdata, obj.m_pdata + this->pool_size, this->m_pdata);
        fmt::println("copy ctor");
    }

    memory_pool(memory_pool&& obj) {
        this->m_pdata = std::exchange(obj.m_pdata, nullptr);
        fmt::println("move ctor");
    }

    auto operator=(memory_pool const& obj) -> memory_pool& {
        if (&obj == this)
            return *this;
        char* new_pdata = new char[pool_size];
        std::copy(obj.m_pdata, obj.m_pdata + this->pool_size, new_pdata);
        delete[] this->m_pdata;
        this->m_pdata = new_pdata;
        fmt::println("copy assign");
        return *this;
    }

    auto operator=(memory_pool&& obj) -> memory_pool& {
        if (&obj == this)
            return *this;
        delete[] this->m_pdata;
        std::exchange(obj.m_pdata, nullptr);
        fmt::println("move assign");
        return *this;
    }

    ~memory_pool() {
        delete[] m_pdata;
        fmt::println("dtor");
    }

private:
    char* m_pdata{nullptr};
};

// lvalue or rvalue test
static void left_or_right_test(int& x) { fmt::println("left"); }
static void left_or_right_test(int&& x) { fmt::println("right"); }

template <typename T>
void forward(T&& x) {
    left_or_right_test(std::forward<T>(x));
    fmt::println("{}", __PRETTY_FUNCTION__);
}
