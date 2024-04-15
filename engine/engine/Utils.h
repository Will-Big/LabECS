#pragma once
#include "Systems.h"
#include "MetaCtxs.h"
#include "SystemTemplates.h"
#include "ComponentTemplates.h"

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

namespace core
{
	inline void RegisterCoreMetaData()
	{
		// 시스템 메타 데이터 등록
		{
			entt::meta<TransformSystem>(systemMetaCtx)
				META_TYPE_HELPER(TransformSystem)
				META_SYSTEM_FUNC_HELPER(TransformSystem);

			entt::meta<PhysicsSystem>(systemMetaCtx)
				META_TYPE_HELPER(PhysicsSystem)
				META_SYSTEM_FUNC_HELPER(PhysicsSystem);

			entt::meta<AnimationSystem>(systemMetaCtx)
				META_TYPE_HELPER(AnimationSystem)
				META_SYSTEM_FUNC_HELPER(AnimationSystem);

			entt::meta<EventTestSystem>(systemMetaCtx)
				META_TYPE_HELPER(EventTestSystem)
				META_SYSTEM_FUNC_HELPER(EventTestSystem);
		}

		// 이벤트 메타 데이터 등록
		{
			entt::meta<OnStartSystem>(eventMetaCtx)
				META_TYPE_HELPER(OnStartSystem);

			entt::meta<OnFinishSystem>(eventMetaCtx)
				META_TYPE_HELPER(OnFinishSystem);

			entt::meta<OnCreateEntity>(eventMetaCtx)
				META_TYPE_HELPER(OnCreateEntity);

			entt::meta<OnDestroyEntity>(eventMetaCtx)
				META_TYPE_HELPER(OnDestroyEntity);
		}

		// 컴포넌트 메타 데이터 등록
		{
			entt::meta<Transform>(componentMetaCtx)
				META_TYPE_HELPER(Transform)
				META_COMPONENT_FUNC_HELPER(Transform)
				META_COMPONENT_MEMBER_HELPER(Transform::position, "position")
				META_COMPONENT_MEMBER_HELPER(Transform::rotation, "rotation")
				META_COMPONENT_MEMBER_HELPER(Transform::scale, "scale")
				META_COMPONENT_MEMBER_HELPER(Transform::localMatrix, "localMatrix")
				META_COMPONENT_MEMBER_HELPER(Transform::worldMatrix, "worldMatrix");

			entt::meta<Relationship>(componentMetaCtx)
				META_TYPE_HELPER(Relationship)
				META_COMPONENT_FUNC_HELPER(Relationship)
				META_COMPONENT_MEMBER_HELPER(Relationship::parent, "parent");

			entt::meta<Name>(componentMetaCtx)
				META_TYPE_HELPER(Name)
				META_COMPONENT_FUNC_HELPER(Name)
				META_COMPONENT_MEMBER_HELPER(Name::name, "name");

			entt::meta<Rigidbody>(componentMetaCtx)
				META_TYPE_HELPER(Rigidbody)
				META_COMPONENT_FUNC_HELPER(Rigidbody)
				META_COMPONENT_MEMBER_HELPER(Rigidbody::mass, "mass")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::drag, "drag")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::angularDrag, "angularDrag")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::useGravity, "useGravity")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::isKinematic, "isKinematic")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::interpolation, "interpolation")
				META_COMPONENT_MEMBER_HELPER(Rigidbody::constraints, "constraints");

			entt::meta<ColliderCommon>(componentMetaCtx)
				META_TYPE_HELPER(ColliderCommon)
				META_COMPONENT_FUNC_HELPER(ColliderCommon)
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::isTrigger, "isTrigger")
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::material, "shape")
				META_COMPONENT_MEMBER_HELPER(ColliderCommon::sharedMaterial, "sharedMaterial");

			entt::meta<BoxCollider>(componentMetaCtx)
				META_TYPE_HELPER(BoxCollider)
				META_COMPONENT_FUNC_HELPER(BoxCollider)
				META_COMPONENT_MEMBER_HELPER(BoxCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(BoxCollider::size, "size");

			entt::meta<SphereCollider>(componentMetaCtx)
				META_TYPE_HELPER(SphereCollider)
				META_COMPONENT_FUNC_HELPER(SphereCollider)
				META_COMPONENT_MEMBER_HELPER(SphereCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(SphereCollider::radius, "radius");

			entt::meta<CapsuleCollider>(componentMetaCtx)
				META_TYPE_HELPER(CapsuleCollider)
				META_COMPONENT_FUNC_HELPER(CapsuleCollider)
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::center, "center")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::radius, "radius")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::height, "height")
				META_COMPONENT_MEMBER_HELPER(CapsuleCollider::direction, "direction");
		}
	}
}
