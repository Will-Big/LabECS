#pragma once

namespace core
{
	struct Transform
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		Matrix localMatrix;
		Matrix worldMatrix;
	};

	struct Relationship
	{
		entt::entity parent = entt::null;
	};

	struct Name
	{
		std::string name;
	};
}




