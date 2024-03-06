#include <algorithm>
#include <memory>
#include <spdlog/fmt/bundled/core.h>
#include <utility>

// class with ptr and move ctor
class memory_pool {
public:
    static int constexpr pool_size = 4096;

    memory_pool() : m_pdata(new char[pool_size]) { fmt::print("ctor\n"); }

    memory_pool(memory_pool const& obj) : m_pdata(new char[pool_size]) {
        std::copy(obj.m_pdata, obj.m_pdata + this->pool_size, this->m_pdata);
        fmt::print("copy ctor\n");
    }

    memory_pool(memory_pool&& obj) noexcept {
        this->m_pdata = std::exchange(obj.m_pdata, nullptr);
        fmt::print("move ctor\n");
    }

    auto operator=(memory_pool const& obj) -> memory_pool& {
        if (&obj == this)
            return *this;
        char* new_pdata = new char[pool_size];
        std::copy(obj.m_pdata, obj.m_pdata + this->pool_size, new_pdata);
        delete[] this->m_pdata;
        this->m_pdata = new_pdata;
        fmt::print("copy assign\n");
        return *this;
    }

    auto operator=(memory_pool&& obj) noexcept -> memory_pool& {
        if (&obj == this)
            return *this;
        delete[] this->m_pdata;
        std::exchange(obj.m_pdata, nullptr);
        fmt::print("move assign\n");
        return *this;
    }

    ~memory_pool() {
        delete[] m_pdata;
        fmt::print("dtor\n");
    }

private:
    char* m_pdata{nullptr};
};

class SString {
public:
    using uptr = std::unique_ptr<SString>;
    using sptr = std::shared_ptr<SString>;

    SString() = default;
    SString(char const* s) : data_(new char[strlen(s) + 1]) {
        strcpy(data_.get(), s);
    }

    SString(SString const& s) : data_(new char[strlen(s.data_.get()) + 1]) {
        strcpy(data_.get(), s.data_.get());
    }

    auto operator=(SString const& s) -> SString& {
        data_.reset(new char[strlen(s.data_.get()) + 1]);
        strcpy(data_.get(), s.data_.get());
        return *this;
    };

    SString(SString&&) noexcept = default;

    auto operator=(SString&&) noexcept -> SString& = default;

    ~SString() = default;

private:
    std::unique_ptr<char[]> data_;
};

// lvalue or rvalue test
static void left_or_right_test(int& x) { fmt::print("left\n"); }
static void left_or_right_test(int&& x) { fmt::print("right\n"); }

template <typename T>
void forward(T&& x) {
    left_or_right_test(std::forward<T>(x));
    fmt::print("{}\n", __PRETTY_FUNCTION__);
}
