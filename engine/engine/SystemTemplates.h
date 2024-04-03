#pragma once

namespace engine
{
	class Scene;

	template <typename T>
	void LoadSystem(Scene* scene)
	{
		constexpr auto name = entt::type_name<T>().value();


	}
}
