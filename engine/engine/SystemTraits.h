#pragma once

template <typename T>
struct SystemTraits;

#define DEFINE_SYSTEM_TRAITS(SystemClass, SystemTypeValue) \
    template<> \
    struct SystemTraits<SystemClass> \
    { \
        static constexpr SystemType type = SystemTypeValue; \
        static constexpr auto name = #SystemClass; \
    };

template <typename T>
concept IsCallableSystem = requires(T t, entt::registry & registry, float tick)
{
    { t(registry, tick) } -> std::same_as<void>;
};