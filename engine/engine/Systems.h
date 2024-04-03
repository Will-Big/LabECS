#pragma once
#include "Scene.h"
#include "SystemTraits.h"

namespace engine
{
	class TransformSystem
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(TransformSystem, SystemType::Update)


	class TestUpdateSystem1
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(TestUpdateSystem1, SystemType::Update)


	class TestUpdateSystem2
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(TestUpdateSystem2, SystemType::Update)

	class Graphics;

	class AnimationRenderSystem
	{
	public:
		void operator()(entt::registry& registry, Graphics& graphics, float tick);
	};
	DEFINE_SYSTEM_TRAITS(AnimationRenderSystem, SystemType::Render)
}
