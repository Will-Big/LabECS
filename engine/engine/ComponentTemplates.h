#pragma once
#include "Components.h"

namespace engine
{
	class cereal::JSONOutputArchive;
	class cereal::JSONInputArchive;

	template<typename Archive>
	void serialize(Archive& archive, engine::Transform& transform) {
		archive(CEREAL_NVP(transform.x), CEREAL_NVP(transform.y), CEREAL_NVP(transform.z));
	}

	template<typename Archive>
	void serialize(Archive& archive, engine::Relationship& relationship) {
		archive(CEREAL_NVP(relationship.parent));
	}

	template <typename T>
	void LoadSnapshot(entt::snapshot_loader* loader, cereal::JSONInputArchive* archive)
	{
		std::cout << "LoadSnapshot" << '\n';
		loader->get<T>(*archive);
	}

	template <typename T>
	void SetSnapshot(entt::snapshot* snapshot, cereal::JSONOutputArchive* archive)
	{
		std::cout << "SetSnapshot" << '\n';
		snapshot->get<T>(*archive);
	}
}
