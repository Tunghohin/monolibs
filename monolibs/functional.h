#ifndef __MONOLIBS_FUNCTIONAL_H
#define __MONOLIBS_FUNCTIONAL_H

#include <functional>
#include <memory>
#include <utility>

namespace mono {

template <typename Fn>
class function {
    static_assert(!std::is_same_v<Fn, Fn>, "not a valid function signature");
};

template <typename Ret, typename... Args>
class function<Ret(Args...)> {
public:
    class function_base {
    public:
        virtual auto call(Args... args) -> Ret = 0;
        virtual ~function_base() = default;
    };

    template <typename Fn>
    class function_impl : public function_base {
    public:
        function_impl(Fn f) : f(std::move(f)) {}
        virtual auto call(Args... args) -> Ret override {
            return std::invoke(f, std::forward<Args>(args)...);
        }

        Fn f;
    };

public:
    function() = default;
    template <typename Fn>
        requires(std::is_invocable_r_v<Ret, Fn, Args...> &&
                 !std::is_same_v<std::decay_t<Fn>, function>)
    function(Fn f)
        : m_func(std::make_shared<function_impl<Fn>>(std::move(f))) {}

    auto operator()(Args... args) -> Ret {
        return m_func->call(std::forward<Args>(args)...);
    }

private:
    std::shared_ptr<function_base> m_func;
};

} // namespace mono

#endif
