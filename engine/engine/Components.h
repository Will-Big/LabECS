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
		archive(CEREAL_NVP(transform.x), CEREAL_NVP(transform.y), CEREAL_NVP(transform.z));
	}

	template<typename Archive>
	void serialize(Archive& archive, engine::Relationship& relationship) {
		archive(CEREAL_NVP(relationship.parent));
	}
}




