#pragma once
#include "CoreComponents.h"
#include "CorePhysicsComponents.h"

namespace cereal
{
	template <class Archive>
	void serialize(Archive& archive, DirectX::SimpleMath::Vector3& vector)
	{
		archive(
			CEREAL_NVP(vector.x),
			CEREAL_NVP(vector.y),
			CEREAL_NVP(vector.z)
		);
	}

	template <class Archive>
	void serialize(Archive& archive, DirectX::SimpleMath::Quaternion& quaternion)
	{
		archive(
			CEREAL_NVP(quaternion.x),
			CEREAL_NVP(quaternion.y),
			CEREAL_NVP(quaternion.z),
			CEREAL_NVP(quaternion.w)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::Transform& transform)
	{
		archive(
			CEREAL_NVP(transform.position),
			CEREAL_NVP(transform.rotation),
			CEREAL_NVP(transform.scale)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::Name& name)
	{
		archive(CEREAL_NVP(name.name));
	}

	template <typename Archive>
	void serialize(Archive& archive, core::Relationship& relationship)
	{
		archive(CEREAL_NVP(relationship.parent));
	}

	template <typename Archive>
	void serialize(Archive& archive, core::Rigidbody& rigidbody)
	{
		archive(
			CEREAL_NVP(rigidbody.mass),
			CEREAL_NVP(rigidbody.drag),
			CEREAL_NVP(rigidbody.angularDrag),
			CEREAL_NVP(rigidbody.useGravity),
			CEREAL_NVP(rigidbody.isKinematic),
			CEREAL_NVP(rigidbody.interpolation),
			CEREAL_NVP(rigidbody.constraints)
		);
	}

	template <class Archive>
	void serialize(Archive& archive, core::ColliderCommon::PhysicMaterial& material)
	{
		archive(
			CEREAL_NVP(material.dynamicFriction),
			CEREAL_NVP(material.staticFriction),
			CEREAL_NVP(material.bounciness)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::ColliderCommon& collider)
	{
		archive(
			CEREAL_NVP(collider.isTrigger),
			CEREAL_NVP(collider.material),
			CEREAL_NVP(collider.sharedMaterial)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::BoxCollider& boxCollider)
	{
		archive(
			CEREAL_NVP(boxCollider.center),
			CEREAL_NVP(boxCollider.size)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::SphereCollider& sphereCollider)
	{
		archive(
			CEREAL_NVP(sphereCollider.center),
			CEREAL_NVP(sphereCollider.radius)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::CapsuleCollider& capsuleCollider)
	{
		archive(
			CEREAL_NVP(capsuleCollider.center),
			CEREAL_NVP(capsuleCollider.radius),
			CEREAL_NVP(capsuleCollider.height),
			CEREAL_NVP(capsuleCollider.direction)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::MeshCollider& meshCollider)
	{
		archive(
			CEREAL_NVP(meshCollider.convex),
			CEREAL_NVP(meshCollider.sharedMesh),
			CEREAL_NVP(meshCollider.cookingOptions)
		);
	}

	template <typename Archive>
	void serialize(Archive& archive, core::Tag& tag)
	{
		archive(CEREAL_NVP(tag.id));
	}
}

namespace core
{
	class cereal::JSONOutputArchive;
	class cereal::JSONInputArchive;

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

	template <typename T>
	void Assign(entt::registry* registry, const entt::entity* entity, entt::meta_any* component)
	{
		registry->emplace_or_replace<T>(*entity, component->cast<T>());
	}

}
