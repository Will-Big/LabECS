#pragma once

#define DEFINE_SYSTEM_TRAITS(SystemClass) \
    template<> \
    struct core::SystemTraits<SystemClass> \
    { \
        static constexpr auto name = #SystemClass; \
    };


namespace core
{
	template <typename T>
	struct SystemTraits;

	// 기본 생성자와 SystemTraits::name 을 멤버로 가짐
	template <typename T>
	concept HasSystemTraits = requires
	{
		{ SystemTraits<T>::name } -> std::convertible_to<const char*>;
	} && std::is_default_constructible_v<T>;

	enum class SystemType
	{
		None,		
		Update,
		FixedUpdate,
		Render,
	};
}