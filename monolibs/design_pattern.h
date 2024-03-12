#ifndef __DESIGN_PATTERN_H
#define __DESIGN_PATTERN_H

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
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
enum SingletonLifeTime {
    Default,
    Extend,
};

template <typename T, SingletonLifeTime LT = SingletonLifeTime::Default>
class Singleton;

template <typename T>
class Singleton<T, SingletonLifeTime::Default> {
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
class Singleton<T, SingletonLifeTime::Extend> {
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

protected:
    Singleton() = default;
    virtual ~Singleton() {
        pInstance_ = nullptr;
        destoryed_ = true;
    }

    static T* pInstance_;
    static bool destoryed_;

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
};

template <typename T>
T* Singleton<T, SingletonLifeTime::Extend>::pInstance_ = nullptr;

template <typename T>
bool Singleton<T, SingletonLifeTime::Extend>::destoryed_ = false;

template <typename T>
mono::spin_lock Singleton<T, SingletonLifeTime::Extend>::lock_;

// OKDP factory method
template <typename AbstractProduct>
class ObjectFactory : public Singleton<ObjectFactory<AbstractProduct>> {
    using this_type = ObjectFactory<AbstractProduct>;

public:
    template <typename T>
        requires std::derived_from<T, AbstractProduct>
    auto register_type(std::string const& key) -> void {
        map_.emplace(key, [] { return new T(); });
    }

    template <typename T, typename... Args>
        requires std::derived_from<T, AbstractProduct>
    auto register_type(std::string const& key, Args... args) -> void {
        map_.emplace(key, [=] { return new T(std::forward<Args>(args)...); });
    }

    auto unregister(std::string const& key) -> void {
        this_type::instance().map_.erase(key);
    }

    auto create(std::string const& key) -> AbstractProduct* {
        if (this_type::instance().map_.find(key) ==
            this_type::instance().map_.end()) {
            return nullptr;
        }
        return this_type::instance().map_[key]();
    }

    auto create_unique(std::string const& key)
        -> std::unique_ptr<AbstractProduct> {
        return std::unique_ptr<AbstractProduct>(create(key));
    }

    auto create_shared(std::string const& key)
        -> std::shared_ptr<AbstractProduct> {
        return std::shared_ptr<AbstractProduct>(create(key));
    }

private:
    std::unordered_map<std::string, std::function<AbstractProduct*()>> map_;
};

} // namespace mono

#endif
