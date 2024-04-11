#pragma once

namespace core
{
	class Scene;
	class Entity;

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

	struct OnStartEntity
	{
		const Entity* entity = nullptr;

		OnStartEntity(const Entity& entity) : entity(&entity) {}
	};

	struct OnDestroyEntity
	{
		const Entity* entity = nullptr;

		OnDestroyEntity(const Entity& entity) : entity(&entity) {}
	};
}
