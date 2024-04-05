#pragma once
#include "Components.h"

namespace core
{
	inline Rigidbody::Constraints operator|(Rigidbody::Constraints lhs, Rigidbody::Constraints rhs)
	{
		return static_cast<Rigidbody::Constraints>(
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(lhs) |
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(rhs));
	}

	inline Rigidbody::Constraints operator&(Rigidbody::Constraints lhs, Rigidbody::Constraints rhs)
	{
		return static_cast<Rigidbody::Constraints>(
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(lhs) &
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(rhs));
	}

	inline Rigidbody::Constraints operator^(Rigidbody::Constraints lhs, Rigidbody::Constraints rhs)
	{
		return static_cast<Rigidbody::Constraints>(
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(lhs) ^
			static_cast<std::underlying_type_t<Rigidbody::Constraints>>(rhs));
	}

	inline Rigidbody::Constraints operator~(Rigidbody::Constraints rhs)
	{
		return static_cast<Rigidbody::Constraints>(
			~static_cast<std::underlying_type_t<Rigidbody::Constraints>>(rhs));
	}

	inline Rigidbody::Constraints& operator|=(Rigidbody::Constraints& lhs, Rigidbody::Constraints rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	inline Rigidbody::Constraints& operator&=(Rigidbody::Constraints& lhs, Rigidbody::Constraints rhs)
	{
		lhs = lhs & rhs;
		return lhs;
	}

	inline Rigidbody::Constraints& operator^=(Rigidbody::Constraints& lhs, Rigidbody::Constraints rhs)
	{
		lhs = lhs ^ rhs;
		return lhs;
	}
}
