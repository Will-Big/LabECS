#pragma once
#include "CoreTagsAndLayers.h"
#include "MetaCtxs.h"
#include "CoreSystems.h"
#include "SystemTemplates.h"
#include "CorePhysicsComponents.h"
#include "ComponentTemplates.h"
#include "PhysicsScene.h"

#define META_TYPE_HELPER(class) \
	.type(entt::type_hash<class>::value())

#define META_COMPONENT_FUNC_HELPER(class) \
	.func<&core::Assign<class>>("Assign"_hs) \
	.func<&core::SaveSnapshot<class>>("SaveSnapshot"_hs) \
	.func<&core::LoadSnapshot<class>>("LoadSnapshot"_hs) \
	.func<&core::SavePrefabSnapshot<class>>("SavePrefabSnapshot"_hs) \
	.func<&core::LoadPrefabSnapshot<class>>("LoadPrefabSnapshot"_hs) \

#define META_COMPONENT_MEMBER_HELPER(memberAddress, name) \
	.data<&##memberAddress>(name##_hs) \
	.prop("name"_hs, name) \

#define META_SYSTEM_FUNC_HELPER(class) \
	.func<&core::LoadSystem<class>>("LoadSystem"_hs) \

#define META_LAYER_MEMBER_HELPER(class) \
	.prop("index"_hs, class::index) \

#define ADD_PHYSICS_LAYER(class) \
	PhysicsScene::AddLayer(class::id)

namespace core
{
	inline void RegisterCoreMetaData()
	{
		// 시스템 메타 데이터 등록
		{
			entt::meta<TransformSystem>(global::systemMetaCtx)
				META_TYPE_HELPER(TransformSystem)
				META_SYSTEM_FUNC_HELPER(TransformSystem);

			entt::meta<PhysicsSystem>(global::systemMetaCtx)
				META_TYPE_HELPER(PhysicsSystem)
				META_SYSTEM_FUNC_HELPER(PhysicsSystem);

			entt::meta<AnimationSystem>(global::systemMetaCtx)
				META_TYPE_HELPER(AnimationSystem)
				META_SYSTEM_FUNC_HELPER(AnimationSystem);

			entt::meta<CollisionTesterSystem>(global::systemMetaCtx)
				META_TYPE_HELPER(CollisionTesterSystem)
				META_SYSTEM_FUNC_HELPER(CollisionTesterSystem);
		}

		// 이벤트 메타 데이터 등록
		{
			entt::meta<OnStartSystem>(global::eventMetaCtx)
				META_TYPE_HELPER(OnStartSystem);

			entt::meta<OnFinishSystem>(global::eventMetaCtx)
				META_TYPE_HELPER(OnFinishSystem);

			entt::meta<OnCreateEntity>(global::eventMetaCtx)
				META_TYPE_HELPER(OnCreateEntity);

			entt::meta<OnDestroyEntity>(global::eventMetaCtx)
				META_TYPE_HELPER(OnDestroyEntity);
		}

		// 컴포넌트 메타 데이터 등록
		{
			entt::meta<Transform>(global::componentMetaCtx)
				META_TYPE_HELPER(Transform)
				META_COMPONENT_FUNC_HELPER(Transform)
				META_COMPONENT_MEMBER_HELPER(Transform::position, "position")
				META_COMPONENT_MEMBER_HELPER(Transform::rotation, "rotation")
				META_COMPONENT_MEMBER_HELPER(Transform::scale, "scale")
				META_COMPONENT_MEMBER_HELPER(Transform::localMatrix, "localMatrix")
				META_COMPONENT_MEMBER_HELPER(Transform::worldMatrix, "worldMatrix");

			entt::meta<Relationship>(global::componentMetaCtx)
				META_TYPE_HELPER(Relationship)
				META_COMPONENT_FUNC_HELPER(Relationship)
				META_COMPONENT_MEMBER_HELPER(Relationship::parent, "parent");

			entt::meta<Name>(global::componentMetaCtx)
				META_TYPE_HELPER(Name)
				META_COMPONENT_FUNC_HELPER(Name)
				META_COMPONENT_MEMBER_HELPER(Name::name, "name");

			entt::meta<Rigidbody>(global::componentMetaCtx)
				META_TYPE_HELPER(Rigidbody)
				META_COMPONENT_FUNC_HELPER(Rigidbody)
				META_COMPONENT_MEMBER_HELPER(Rigidbody::mass, "mass")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::drag, "drag")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::angularDrag, "angularDrag")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::useGravity, "useGravity")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::isKinematic, "isKinematic")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::interpolation, "interpolation")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::constraints, "constraints");

			entt::meta<ColliderCommon>(global::componentMetaCtx)
				META_TYPE_HELPER(ColliderCommon)
				META_COMPONENT_FUNC_HELPER(ColliderCommon)
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::isTrigger, "isTrigger")
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::material, "shape")
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::sharedMaterial, "sharedMaterial");

			entt::meta<BoxCollider>(global::componentMetaCtx)
				META_TYPE_HELPER(BoxCollider)
				META_COMPONENT_FUNC_HELPER(BoxCollider)
				META_COMPONENT_MEMBER_HELPER(BoxCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(BoxCollider::size, "size");

			entt::meta<SphereCollider>(global::componentMetaCtx)
				META_TYPE_HELPER(SphereCollider)
				META_COMPONENT_FUNC_HELPER(SphereCollider)
				META_COMPONENT_MEMBER_HELPER(SphereCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(SphereCollider::radius, "radius");

			entt::meta<CapsuleCollider>(global::componentMetaCtx)
				META_TYPE_HELPER(CapsuleCollider)
				META_COMPONENT_FUNC_HELPER(CapsuleCollider)
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::radius, "radius")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::height, "height")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::direction, "direction");

			entt::meta<Tag>(global::componentMetaCtx)
				META_TYPE_HELPER(Tag)
				META_COMPONENT_FUNC_HELPER(Tag)
				META_COMPONENT_MEMBER_HELPER(Tag::id, "id");

			entt::meta<Layer>(global::componentMetaCtx)
				META_TYPE_HELPER(Layer)
				META_COMPONENT_FUNC_HELPER(Layer)
				META_COMPONENT_MEMBER_HELPER(Layer::id, "id");
		}

		// 태그 메타 데이터 등록
		{
			entt::meta<tag::Untagged>(global::tagMetaCtx)
				META_TYPE_HELPER(tag::Untagged);

			entt::meta<tag::Respawn>(global::tagMetaCtx)
				META_TYPE_HELPER(tag::Respawn);

			entt::meta<tag::Finish>(global::tagMetaCtx)
				META_TYPE_HELPER(tag::Finish);

			entt::meta<tag::MainCamera>(global::tagMetaCtx)
				META_TYPE_HELPER(tag::MainCamera);

			entt::meta<tag::Player>(global::tagMetaCtx)
				META_TYPE_HELPER(tag::Player);
		}

		// 레이어 메타 데이터 등록 및 물리 레이어 추가
		{
			entt::meta<layer::Default>(global::layerMetaCtx)
				META_TYPE_HELPER(layer::Default)
				META_LAYER_MEMBER_HELPER(layer::Default);
			ADD_PHYSICS_LAYER(layer::Default);
		}
	}
}
