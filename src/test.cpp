#include <extlibs/ranges_ext.h>
#include <fmt/format.h>
#include <monolibs/utility.h>
#include <vector>

template <std::ranges::range R>
auto func(R&& r) -> void {
    for (auto item : r) {
        fmt::println("{}", item);
    }
}

auto main() -> int {
    auto v = std::vector<int>{1, 2, 3} | tl::to<std::vector<int>>();
    func(v);
}