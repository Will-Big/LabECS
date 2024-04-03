#pragma once
#include "Scene.h"
#include "SystemTraits.h"
#include "SystemTemplates.h"

namespace engine
{
	class Graphics;

	class TransformSystem
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(TransformSystem, SystemType::Update)

	class PhysicsSystem
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(PhysicsSystem, SystemType::FixedUpdate);

	class AnimationRenderSystem
	{
	public:
		void operator()(entt::registry& registry, Graphics& graphics, float tick);
	};
	DEFINE_SYSTEM_TRAITS(AnimationRenderSystem, SystemType::Render)
}
