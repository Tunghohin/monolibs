#include <functional.h>
#include <iostream>

typedef void (*pfunc_t)(void*);

typedef struct pfunc_args_t {
    int x;
} pfunc_args_t;

auto say_hello(void* args) {
    std::cout << "hello" << std::endl;
    if (args != NULL) {
        pfunc_args_t pfunc_args = *(pfunc_args_t*)args;
        std::cout << pfunc_args.x << std::endl;
    }
}

auto say_world(void* args) {
    std::cout << "world" << std::endl;
    if (args != NULL) {
        pfunc_args_t pfunc_args = *(pfunc_args_t*)args;
        std::cout << pfunc_args.x << std::endl;
    }
}

auto do_func(pfunc_t func, void* args) { func(args); }

auto main() -> int {
    pfunc_t func = say_hello;
    pfunc_args_t args = {2};

    do_func(func, &args);
    do_func(say_world, NULL);

    int res = mono::func(7);
    std::cout << res << std::endl;

    return 0;
}
