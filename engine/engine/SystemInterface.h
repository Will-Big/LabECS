﻿#pragma once

namespace core
{
	class Graphics;

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
}
