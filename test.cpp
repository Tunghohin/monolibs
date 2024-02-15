#include "utility.h"
#include <algorithm>
#include <linear_algebra.h>
#include <memory>
#include <numeric>
#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <type_traits>
#include <utility>
#include <vector>

auto takes_and_gives(std::unique_ptr<std::string> uptr)
    -> std::unique_ptr<std::string> {
    *uptr += "asdf";
    return uptr;
}

auto main() -> int {
    auto uptr = std::make_unique<std::string>("aaa");
    mono::lrvalue_test(uptr);
    mono::lrvalue_test(takes_and_gives(std::move(uptr)));

    return 0;
}
