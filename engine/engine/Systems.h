#pragma once
#include "Scene.h"
#include "SystemTraits.h"
#include "SystemInterface.h"

namespace core
{
	class PhysicsScene;

	class TransformSystem : public IUpdateSystem
	{
	public:
		void operator()(entt::registry& registry, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(TransformSystem, SystemType::Update)

	class PhysicsSystem : public IFixedSystem
	{
	public:
		void operator()(entt::registry& registry, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(PhysicsSystem, SystemType::FixedUpdate);

	class AnimationSystem : public IUpdateSystem, public IRenderSystem
	{
	public:
		void operator()(entt::registry& registry, float tick) override;
		void operator()(entt::registry& registry, Graphics& graphics, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(AnimationSystem, SystemType::Update | SystemType::Render)
}
