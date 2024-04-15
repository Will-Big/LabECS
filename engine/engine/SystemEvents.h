#pragma once

#include "Entity.h"

namespace core
{
	class Scene;

	struct OnStartSystem
	{
		Scene* scene = nullptr;

		OnStartSystem(Scene& scene) : scene(&scene) {}
	};

	struct OnFinishSystem
	{
		Scene* scene = nullptr;

		OnFinishSystem(Scene& scene) : scene(&scene) {}
	};

	struct OnCreateEntity
	{
		Entity entity;

		OnCreateEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnCreateEntity(const Entity& entity) : entity(entity) {}
	};

	struct OnDestroyEntity
	{
		Entity entity;

		OnDestroyEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnDestroyEntity(const Entity& entity) : entity(entity) {}
	};

	struct OnCollisionEnter
	{
		Entity entity1;
		Entity entity2;

		OnCollisionEnter(entt::entity handle1, entt::entity handle2, entt::registry& registry)
			: entity1(handle1, registry), entity2(handle2, registry) {}
		OnCollisionEnter(const Entity& entity1, const Entity& entity2)
			: entity1(entity1), entity2(entity2) {}
	};

	struct OnCollisionStay
	{
		Entity entity1;
		Entity entity2;

		OnCollisionStay(entt::entity handle1, entt::entity handle2, entt::registry& registry)
			: entity1(handle1, registry), entity2(handle2, registry) {}
		OnCollisionStay(const Entity& entity1, const Entity& entity2)
			: entity1(entity1), entity2(entity2) {}
	};

	struct OnCollisionExit
	{
		Entity entity1;
		Entity entity2;

		OnCollisionExit(entt::entity handle1, entt::entity handle2, entt::registry& registry)
			: entity1(handle1, registry), entity2(handle2, registry) {}
		OnCollisionExit(const Entity& entity1, const Entity& entity2)
			: entity1(entity1), entity2(entity2) {}
	};
}
