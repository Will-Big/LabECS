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
}
