#pragma once

namespace core
{
	class Scene;

	template <typename T> requires HasSystemTraits<T>
	void LoadSystem(Scene* scene, std::vector<std::string>* systemNames)
	{
		constexpr auto name = SystemTraits<T>::name;
		constexpr auto type = SystemTraits<T>::type;

        auto it = std::ranges::find(*systemNames, name);

        if (it != systemNames->end()) 
			scene->RegisterSystem<T>();
	}
}
