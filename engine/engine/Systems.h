#pragma once
#include "Scene.h"
#include "SystemTraits.h"
#include "SystemTemplates.h"

namespace engine
{
	class IUpdateSystem
	{
	public:
		virtual ~IUpdateSystem() = default;
		virtual void operator()(entt::registry& registry, float tick) = 0;
	};

	class IFixedSystem
	{
	public:
		virtual ~IFixedSystem() = default;
		virtual void operator()(entt::registry& registry, float tick) = 0;
	};

	class IRenderSystem
	{
	public:
		virtual ~IRenderSystem() = default;
		virtual void operator()(entt::registry& registry, Graphics& graphics, float tick) = 0;
	};


	class Graphics;

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

	class AnimationRenderSystem : public IUpdateSystem, public IRenderSystem
	{
	public:
		void operator()(entt::registry& registry, float tick) override;
		void operator()(entt::registry& registry, Graphics& graphics, float tick) override;
	};
	DEFINE_SYSTEM_TRAITS(AnimationRenderSystem, SystemType::Update | SystemType::Render)
}
