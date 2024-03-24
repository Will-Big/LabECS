#pragma once

constexpr entt::hashed_string operator"" _hs(const char* str, std::size_t len)
{
	return entt::hashed_string{ str, len };
}