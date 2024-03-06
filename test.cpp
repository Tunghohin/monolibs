#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <design_pattern.h>
#include <iostream>
#include <memory>
#include <spdlog/fmt/bundled/core.h>
#include <type_traits>
#include <utility>

class Log : public mono::Singleton<Log, true> {
public:
    auto log() -> void { fmt::print("log\n"); }
};

const int n = 5;
int arr[n] = {3, 2, 4, 5, 1};

auto main() -> int {}
