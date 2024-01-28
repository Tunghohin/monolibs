#include <chrono>
#include <fmt/base.h>
#include <functional.h>
#include <iostream>
#include <thread>
#include <type_traits>
#include <utility.h>

void func() {
    std::this_thread::sleep_for(std::chrono::milliseconds(123));
    return;
}

int ret_int() {

    std::this_thread::sleep_for(std::chrono::milliseconds(233));
    return 233;
}

auto main() -> int {
    mono::invoke_and_time(func);
    auto ret = mono::invoke_and_time(ret_int);
    return 0;
}
