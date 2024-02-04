#include <algorithm>
#include <fmt/base.h>
#include <linear_algebra.h>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

auto main() -> int {
    mono::vector2<double> v(2.1, 3.3);
    fmt::println("{}", v[-1]);
    return 0;
}
