#pragma once

#define DEFINE_SYSTEM_TRAITS(SystemClass, SystemTypeValue) \
    template<> \
    struct engine::SystemTraits<SystemClass> \
    { \
        static constexpr SystemType type = SystemTypeValue; \
        static constexpr auto name = #SystemClass; \
    };

namespace engine
{
    template <typename T>
    struct SystemTraits;

    template <typename T>
    concept IsCallableSystem = requires(T t, entt::registry & registry, float tick)
    {
        { t(registry, tick) } -> std::same_as<void>;
    };
}