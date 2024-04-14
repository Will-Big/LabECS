#pragma once

#include "Entity.h"

namespace core
{
	class Scene;

	struct Event {};

	struct OnStartSystem : Event
	{
		Scene* scene = nullptr;

		OnStartSystem(Scene& scene) : scene(&scene) {}
	};

	struct OnFinishSystem : Event
	{
		Scene* scene = nullptr;

		OnFinishSystem(Scene& scene) : scene(&scene) {}
	};

	struct OnCreateEntity : Event
	{
		Entity entity;

		OnCreateEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnCreateEntity(const Entity& entity) : entity(entity) {}
	};

	struct OnDestroyEntity : Event
	{
		Entity entity;

		OnDestroyEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnDestroyEntity(const Entity& entity) : entity(entity) {}
	};
}
