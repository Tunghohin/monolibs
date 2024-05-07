#include <extlibs/ranges_ext.h>
#include <fmt/format.h>
#include <monolibs/utility.h>
#include <vector>

auto main() -> int {
    auto v = std::vector<int>{1, 2, 3} | tl::to<std::vector>();
    for (auto item : v) {
        fmt::println("{}", item);
    }
}