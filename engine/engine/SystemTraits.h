#pragma once

#define DEFINE_SYSTEM_TRAITS(SystemClass) \
    template<> \
    struct core::SystemTraits<SystemClass> \
    { \
        static constexpr auto name = #SystemClass; \
    };

namespace core
{
	class ISystemBase;

	template <typename T>
	struct SystemTraits;

	template <typename T>
	concept HasSystemTraits = requires
	{
		{ SystemTraits<T>::name } -> std::convertible_to<const char*>;
	} && std::is_base_of_v<ISystemBase, T>;

	enum class SystemType
	{
		None,		
		Update,
		FixedUpdate,
		Render,
	};
}
