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
	enum class SystemType;
	template <typename T>
	struct SystemTraits;

	template <typename T>
	concept IsCallableSystem = requires(T t, entt::registry & registry, float tick)
	{
		{ t(registry, tick) } -> std::same_as<void>;
	};

	// SystemTraits가 주어진 T에 대해 정의되어 있고, type과 name을 멤버로 가지는지 확인하는 콘셉트
	template <typename T>
	concept HasSystemTraits = requires
	{
		{ SystemTraits<T>::type } -> std::convertible_to<SystemType>;
		{ SystemTraits<T>::name } -> std::convertible_to<const char*>;
	};
}