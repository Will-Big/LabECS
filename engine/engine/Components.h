#pragma once

namespace engine
{
	struct Transform
	{
		float x;
		float y;
		float z;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, x, y, z)
	};

	struct Parent
	{
		entt::entity parentHandle;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Parent, parentHandle)
	};
}
