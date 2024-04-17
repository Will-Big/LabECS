#pragma once
#include "SystemTraits.h"
#include "SystemEvents.h"
#include "SystemInterface.h"

namespace core
{
	class PhysicsScene;

#pragma region TransformSystem
	class TransformSystem : public ISystem, public IUpdateSystem
	{
	public:
		using ISystem::ISystem;

		void operator()(entt::registry& registry, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(TransformSystem)
#pragma endregion

#pragma region PhysicsSystem
	class PhysicsSystem : public ISystem, public IFixedSystem
	{
	public:
		PhysicsSystem(entt::dispatcher& dispatcher);

		void operator()(entt::registry& registry, float tick) override;

	private:
		void startSystem(const OnStartSystem& event);
		void finishSystem(const OnFinishSystem& event);

		void createEntity(const OnCreateEntity& event);
		void destroyEntity(const OnDestroyEntity& event);

	private:
		std::shared_ptr<PhysicsScene> _physicsScene;
	};
	DEFINE_SYSTEM_TRAITS(PhysicsSystem);
#pragma endregion

#pragma region AnimationSystem
	class AnimationSystem : public ISystem, public IUpdateSystem, public IRenderSystem
	{
	public:
		using ISystem::ISystem;

		void operator()(entt::registry& registry, float tick) override;
		void operator()(entt::registry& registry, Graphics& graphics, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(AnimationSystem)
#pragma endregion

#pragma region CollisionTesterSystem
	class CollisionTesterSystem : public ISystem, public IUpdateSystem, public ICollisionHandler
	{
	public:
		CollisionTesterSystem(entt::dispatcher& dispatcher);

		void operator()(entt::registry& registry, float tick) override {}

	private:
		void startSystem(const OnStartSystem& event);
		void finishSystem(const OnFinishSystem& event);

	public:
		void OnCollisionEnter(const Entity& self, const Entity& other) override;
		void OnCollisionStay(const Entity& self, const Entity& other) override;
		void OnCollisionExit(const Entity& self, const Entity& other) override;
	};
	DEFINE_SYSTEM_TRAITS(CollisionTesterSystem)
#pragma endregion
}
