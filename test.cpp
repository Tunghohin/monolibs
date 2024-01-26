#include <fmt/base.h>
#include <functional.h>
#include <iostream>

typedef void (*pfunc_t)(void*);

typedef struct pfunc_args_t {
    int x;
} pfunc_args_t;

auto say_hello(void* args) -> void {
    std::cout << "hello" << std::endl;
    if (args != NULL) {
        pfunc_args_t pfunc_args = *(pfunc_args_t*)args;
        std::cout << pfunc_args.x << std::endl;
    }
}

auto say_world(void* args) -> void {
    std::cout << "world" << std::endl;
    if (args != NULL) {
        pfunc_args_t pfunc_args = *(pfunc_args_t*)args;
        std::cout << pfunc_args.x << std::endl;
    }
}

auto main() -> int {
    mono::function<void(void*)> func = say_hello;
    func(NULL);
    func = say_world;
    func(NULL);
    func = [](void*) { std::cout << "hohoho" << std::endl; };
    func(NULL);
    return 0;
}
