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

#pragma region EventTestSystem
	class EventTestSystem : public ISystem, public IUpdateSystem
	{
	public:
		EventTestSystem(entt::dispatcher& dispatcher);

		void operator()(entt::registry& registry, float tick) override {}

	private:
		void startSystem(const OnStartSystem& event);
		void finishSystem(const OnFinishSystem& event);
	};
	DEFINE_SYSTEM_TRAITS(EventTestSystem)
#pragma endregion
}
