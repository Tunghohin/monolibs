#ifndef __DESIGN_PATTERN_H
#define __DESIGN_PATTERN_H

#include <utility>

#include "./utility.h"

namespace mono {

// Mayers singleton
class MayersSingleton {
public:
    static auto Instance() -> MayersSingleton& {
        static MayersSingleton obj{};
        return obj;
    }

private:
    MayersSingleton();
    MayersSingleton(MayersSingleton const&) = delete;
    auto operator=(MayersSingleton const&) -> MayersSingleton& = delete;
    MayersSingleton(MayersSingleton&&) = delete;
    auto operator=(MayersSingleton&&) -> MayersSingleton& = delete;
    ~MayersSingleton();
};

// OKDP singleton
template <typename T, bool LongLifeTime = false>
class Singleton;

template <typename T>
class Singleton<T, false> {
public:
    template <typename... Args>
    static auto instance(Args&&... args) -> T& {
        static T obj(std::forward<Args>(args)...);
        return obj;
    }

protected:
    Singleton() = default;
    virtual ~Singleton(){};

private:
    Singleton(Singleton const&) = delete;
    auto operator=(Singleton const&) -> Singleton& = delete;
    Singleton(Singleton&&) = delete;
    auto operator=(Singleton&&) -> Singleton& = delete;
};

template <typename T>
class Singleton<T, true> {
public:
    template <typename... Args>
    static auto instance(Args&&... args) -> T& {
        if (!pInstance_) {
            lock_.lock();
            if (!pInstance_) {
                destoryed_ ? on_dead_reference(std::forward<Args>(args)...)
                           : create(std::forward<Args>(args)...);
            }
            lock_.unlock();
        }
        return *pInstance_;
    }

private:
    template <typename... Args>
    static auto create(Args&&... args) -> void {
        static T obj(std::forward<Args>(args)...);
        pInstance_ = &obj;
    }

    template <typename... Args>
    static auto on_dead_reference(Args&&... args) -> void {
        create(std::forward<Args>(args)...);
        new (pInstance_) T(std::forward<Args>(args)...);
        std::atexit(destory_singleton);
        destoryed_ = false;
    }

    static auto destory_singleton() { pInstance_->~Singleton(); }

    Singleton(Singleton const&) = delete;
    auto operator=(Singleton const&) -> Singleton& = delete;
    Singleton(Singleton&&) = delete;
    auto operator=(Singleton&&) -> Singleton& = delete;

    static mono::spin_lock lock_;

protected:
    Singleton() = default;
    virtual ~Singleton() {
        pInstance_ = nullptr;
        destoryed_ = true;
    }

    static T* pInstance_;
    static bool destoryed_;
};

template <typename T>
T* Singleton<T, true>::pInstance_ = nullptr;

template <typename T>
bool Singleton<T, true>::destoryed_ = false;

template <typename T>
mono::spin_lock Singleton<T, true>::lock_;

} // namespace mono

#endif
