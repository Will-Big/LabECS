#pragma once

#define DEFINE_SYSTEM_TRAITS(SystemClass, SystemTypeValue) \
    template<> \
    struct engine::SystemTraits<SystemClass> \
    { \
        static constexpr engine::SystemType type = SystemTypeValue; \
        static constexpr auto name = #SystemClass; \
    };


namespace engine
{
	enum class SystemType;
	template <typename T>
	struct SystemTraits;

	// SystemTraits가 주어진 T에 대해 정의되어 있고, type과 name을 멤버로 가지는지 확인하는 콘셉트
	template <typename T>
	concept HasSystemTraits = requires
	{
		{ SystemTraits<T>::type } -> std::convertible_to<SystemType>;
		{ SystemTraits<T>::name } -> std::convertible_to<const char*>;
	};

	enum class SystemType
	{
		None			= 0,
		Update			= 1 << 0,	// 0001
		FixedUpdate		= 1 << 1,	// 0010
		Render			= 1 << 2,	// 0100
	};

	// 비트 OR 연산자 오버로딩
	constexpr SystemType operator|(SystemType lhs, SystemType rhs)
	{
		return static_cast<SystemType>(
			static_cast<std::underlying_type_t<SystemType>>(lhs) |
			static_cast<std::underlying_type_t<SystemType>>(rhs)
			);
	}

	// 비트 AND 연산자 오버로딩
	constexpr SystemType operator&(SystemType lhs, SystemType rhs)
	{
		return static_cast<SystemType>(
			static_cast<std::underlying_type_t<SystemType>>(lhs) &
			static_cast<std::underlying_type_t<SystemType>>(rhs)
			);
	}

	// 비트 OR 할당 연산자 오버로딩
	constexpr SystemType& operator|=(SystemType& lhs, SystemType rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	// 비트 AND 할당 연산자 오버로딩
	constexpr SystemType& operator&=(SystemType& lhs, SystemType rhs)
	{
		lhs = lhs & rhs;
		return lhs;
	}
}