#pragma once
#include "Systems.h"
#include "SystemTemplates.h"
#include "ComponentTemplates.h"

#define META_TYPE_HELPER(class) \
	.type(entt::type_hash<class>::value())

#define META_COMPONENT_FUNC_HELPER(class) \
	.func<&core::SaveSnapshot<class>>("SaveSnapshot"_hs) \
	.func<&core::LoadSnapshot<class>>("LoadSnapshot"_hs) \
	.func<&core::SavePrefabSnapshot<class>>("SavePrefabSnapshot"_hs) \
	.func<&core::LoadPrefabSnapshot<class>>("LoadPrefabSnapshot"_hs) \

#define META_SYSTEM_FUNC_HELPER(class) \
	.func<&core::LoadSystem<class>>("LoadSystem"_hs) \

namespace core
{
	inline void RegisterCoreMetaData()
	{
		// 시스템 메타 데이터 등록
		{
			entt::meta<TransformSystem>()
				META_TYPE_HELPER(TransformSystem)
				META_SYSTEM_FUNC_HELPER(TransformSystem);

			entt::meta<PhysicsSystem>()
				META_TYPE_HELPER(PhysicsSystem)
				META_SYSTEM_FUNC_HELPER(PhysicsSystem);

			entt::meta<AnimationSystem>()
				META_TYPE_HELPER(AnimationSystem)
				META_SYSTEM_FUNC_HELPER(AnimationSystem);

			entt::meta<EventTestSystem>()
				META_TYPE_HELPER(EventTestSystem)
				META_SYSTEM_FUNC_HELPER(EventTestSystem);
		}

		// 이벤트 메타 데이터 등록
		{
			entt::meta<OnStartSystem>()
				META_TYPE_HELPER(OnStartSystem)
				.base<Event>();

			entt::meta<OnFinishSystem>()
				META_TYPE_HELPER(OnFinishSystem)
				.base<Event>();

			entt::meta<OnCreateEntity>()
				META_TYPE_HELPER(OnCreateEntity)
				.base<Event>();

			entt::meta<OnDestroyEntity>()
				META_TYPE_HELPER(OnDestroyEntity)
				.base<Event>();
		}

		// 컴포넌트 메타 데이터 등록
		{
			entt::meta<Transform>()
				META_TYPE_HELPER(Transform)
				META_COMPONENT_FUNC_HELPER(Transform)
				.data<&Transform::position>("position"_hs)
				.data<&Transform::rotation>("rotation"_hs)
				.data<&Transform::scale>("scale"_hs)
				.data<&Transform::localMatrix>("localMatrix"_hs)
				.data<&Transform::worldMatrix>("worldMatrix"_hs);

			entt::meta<Relationship>()
				META_TYPE_HELPER(Relationship)
				META_COMPONENT_FUNC_HELPER(Relationship)
				.data<&Relationship::parent>("parent"_hs);

			entt::meta<Name>()
				META_TYPE_HELPER(Name)
				META_COMPONENT_FUNC_HELPER(Name)
				.data<&Name::name>("name"_hs);

			entt::meta<Rigidbody>()
				META_TYPE_HELPER(Rigidbody)
				META_COMPONENT_FUNC_HELPER(Rigidbody)
				.data<&Rigidbody::mass>("mass"_hs)
				.data<&Rigidbody::drag>("drag"_hs)
				.data<&Rigidbody::angularDrag>("angularDrag"_hs)
				.data<&Rigidbody::useGravity>("useGravity"_hs)
				.data<&Rigidbody::isKinematic>("isKinematic"_hs)
				.data<&Rigidbody::interpolation>("interpolation"_hs)
				.data<&Rigidbody::constraints>("constraints"_hs);

			entt::meta<ColliderCommon>()
				META_TYPE_HELPER(ColliderCommon)
				META_COMPONENT_FUNC_HELPER(ColliderCommon)
				.data<&ColliderCommon::isTrigger>("isTrigger"_hs)
				.data<&ColliderCommon::material>("shape"_hs)
				.data<&ColliderCommon::sharedMaterial>("sharedMaterial"_hs);

			entt::meta<BoxCollider>()
				META_TYPE_HELPER(BoxCollider)
				META_COMPONENT_FUNC_HELPER(BoxCollider)
				.data<&BoxCollider::center>("center"_hs)
				.data<&BoxCollider::size>("size"_hs);

			entt::meta<SphereCollider>()
				META_TYPE_HELPER(SphereCollider)
				META_COMPONENT_FUNC_HELPER(SphereCollider)
				.data<&SphereCollider::center>("center"_hs)
				.data<&SphereCollider::radius>("radius"_hs);

			entt::meta<CapsuleCollider>()
				META_TYPE_HELPER(CapsuleCollider)
				META_COMPONENT_FUNC_HELPER(CapsuleCollider)
				.data<&CapsuleCollider::center>("center"_hs)
				.data<&CapsuleCollider::radius>("radius"_hs)
				.data<&CapsuleCollider::height>("height"_hs)
				.data<&CapsuleCollider::direction>("direction"_hs);
		}
	}
}
