#pragma once
#include "Systems.h"
#include "SystemTemplates.h"
#include "ComponentTemplates.h"

namespace core
{
	inline void RegisterMetaData()
	{
		// register system meta data
		{
			entt::meta<TransformSystem>()
				.type(entt::type_hash<TransformSystem>::value())
				.func<&core::LoadSystem<TransformSystem>>("LoadSystem"_hs);

			entt::meta<AnimationSystem>()
				.type(entt::type_hash<AnimationSystem>::value())
				.func<&core::LoadSystem<AnimationSystem>>("LoadSystem"_hs);
		}

		// register component meta data
		{
			entt::meta<Transform>()
				.type(entt::type_hash<Transform>::value())
				.data<&Transform::x>("x"_hs)
				.data<&Transform::y>("y"_hs)
				.data<&Transform::z>("z"_hs)
				.func<&core::SaveSnapshot<Transform>>("SaveSnapshot"_hs)
				.func<&core::LoadSnapshot<Transform>>("LoadSnapshot"_hs)
				.func<&core::SavePrefabSnapshot<Transform>>("SavePrefabSnapshot"_hs)
				.func<&core::LoadPrefabSnapshot<Transform>>("LoadPrefabSnapshot"_hs);


			entt::meta<Relationship>()
				.type(entt::type_hash<Relationship>::value())
				.data<&Relationship::parent>("parent"_hs)
				.func<&core::SaveSnapshot<Relationship>>("SaveSnapshot"_hs)
				.func<&core::LoadSnapshot<Relationship>>("LoadSnapshot"_hs)
				.func<&core::SavePrefabSnapshot<Relationship>>("SavePrefabSnapshot"_hs)
				.func<&core::LoadPrefabSnapshot<Relationship>>("LoadPrefabSnapshot"_hs);
		}
	}
}
