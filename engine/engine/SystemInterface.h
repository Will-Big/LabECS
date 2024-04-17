#pragma once

namespace core
{
	class Scene;
	class Graphics;

	class ISystem
	{
	public:
		ISystem(entt::dispatcher& dispatcher) : _dispatcher(&dispatcher) {}
		virtual ~ISystem() = default;

		entt::dispatcher* _dispatcher = nullptr;
	};

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

	class ICollisionHandler
	{
	public:
		virtual ~ICollisionHandler() = default;
		virtual void OnCollisionEnter(const Entity& self, const Entity& other) = 0;
		virtual void OnCollisionStay(const Entity& self, const Entity& other) = 0;
		virtual void OnCollisionExit(const Entity& self, const Entity& other) = 0;
	};
}
