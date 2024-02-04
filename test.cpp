#include <algorithm>
#include <fmt/base.h>
#include <linear_algebra.h>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

auto main() -> int {
    mono::matrix<float, 3, 4> m;
    m[3][4] = 4;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            fmt::print("{} ", m[i][j]);
        }
        fmt::println("");
    }
    return 0;
}
