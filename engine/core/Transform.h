#pragma once

namespace core
{
	struct Transform
	{
		constexpr ComponentId cid = TYPEID(Transform);
		int a;
		int b;
	};
}
