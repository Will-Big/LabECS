#pragma once
#include <iostream>

#include "Entity.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>


namespace engine
{
	struct Transform
	{
		float x;
		float y;
		float z;

		void Deserialize(entt::registry* registry)
		{
			std::cout << "create!" << std::endl;
			registry->emplace<Transform>(registry->create());
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, x, y, z)
	};

	struct Relationship
	{
		entt::entity parent;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Relationship, parent)
	};


	template<typename Archive>
	void serialize(Archive& archive, engine::Transform& transform) {
		archive(cereal::make_nvp("x", transform.x), cereal::make_nvp("y", transform.y), cereal::make_nvp("z", transform.z));
	}

	template<typename Archive>
	void serialize(Archive& archive, engine::Relationship& relationship) {
		archive(cereal::make_nvp("parent", relationship.parent));
	}
}




