#pragma once
#include "Components.h"

namespace core
{
	class cereal::JSONOutputArchive;
	class cereal::JSONInputArchive;

	template<typename Archive>
	void serialize(Archive& archive, core::Transform& transform) {
		archive(CEREAL_NVP(transform.x), CEREAL_NVP(transform.y), CEREAL_NVP(transform.z));
	}

	template<typename Archive>
	void serialize(Archive& archive, core::Relationship& relationship) {
		archive(CEREAL_NVP(relationship.parent));
	}

	template <typename T>
	void SaveSnapshot(entt::snapshot* snapshot, cereal::JSONOutputArchive* archive)
	{
		snapshot->get<T>(*archive);
	}

	template <typename T>
	void LoadSnapshot(entt::snapshot_loader* loader, cereal::JSONInputArchive* archive)
	{
		loader->get<T>(*archive);
	}

	template <typename T>
	void SavePrefabSnapshot(entt::snapshot* snapshot, cereal::JSONOutputArchive* archive, std::vector<entt::entity>::iterator first, std::vector<entt::entity>::iterator last)
	{
		snapshot->get<T>(*archive, first, last);
	}

	template <typename T>
	void LoadPrefabSnapshot(entt::continuous_loader* loader, cereal::JSONInputArchive* archive)
	{
		loader->get<T>(*archive);
	}

}
