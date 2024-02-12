#include <algorithm>
#include <linear_algebra.h>
#include <numeric>
#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <type_traits>
#include <utility>
#include <vector>

auto main() -> int {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("asdf");
    fmt::print("{}\n", 233);
    return 0;
}
