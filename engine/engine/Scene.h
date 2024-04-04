#pragma once
#include "Entity.h"
#include "SystemTraits.h"

namespace engine
{
	class IFixedSystem;
}

namespace engine
{
	class IUpdateSystem;
	class IRenderSystem;
	class Graphics;

	class Scene
	{
		using SystemInfo = std::pair<SystemType, size_t>;

	public:
		Entity AddEntity();

		template <typename T> requires HasSystemTraits<T>
		void RegisterSystem();

		template <typename T> requires HasSystemTraits<T>
		void RemoveSystem();

		bool SaveScene(const std::string& path);
		bool LoadScene(const std::string& path);

		bool SavePrefab(const std::string& path, engine::Entity& entity);
		bool LoadPrefab(const std::string& path);

		void Run();

	private:
		void UpdateSystemMapIndex(SystemType type, size_t oldIndex, size_t newIndex);

		entt::registry _registry;
		std::multimap<std::string, SystemInfo> _systemMap;

		std::vector<std::unique_ptr<IUpdateSystem>> _updates;
		std::vector<std::unique_ptr<IFixedSystem>> _fixeds;
		std::vector<std::unique_ptr<IRenderSystem>> _renders;
	};

	template <typename T> requires HasSystemTraits<T>
	void Scene::RegisterSystem()
	{
		// 시스템 메타 데이터
		constexpr auto name = SystemTraits<T>::name;
		constexpr auto type = SystemTraits<T>::type;

		if (_systemMap.contains(name))
			return;

		if constexpr (std::is_base_of_v<IUpdateSystem, T>)
		{
			_updates.push_back(std::make_unique<T>());
			_systemMap.insert({ name,  { SystemType::Update, _updates.size() - 1 } });
		}
		if constexpr (std::is_base_of_v<IFixedSystem, T>)
		{
			_fixeds.push_back(std::make_unique<T>());
			_systemMap.insert({ name,  { SystemType::FixedUpdate, _fixeds.size() - 1 } });
		}
		if constexpr (std::is_base_of_v<IRenderSystem, T>)
		{
			_renders.push_back(std::make_unique<T>());
			_systemMap.insert({ name,  { SystemType::Render, _renders.size() - 1 } });
		}
	}

	template <typename T> requires HasSystemTraits<T>
	void Scene::RemoveSystem()
	{
		constexpr auto name = SystemTraits<T>::name;
		constexpr auto type = SystemTraits<T>::type;

		auto range = _systemMap.equal_range(name);

		for (auto it = range.first; it != range.second; ++it)
		{
			auto [sysType, index] = it->second;

			// 시스템 타입에 따라 적절한 컨테이너에서 시스템 삭제
			switch (sysType)
			{
			case SystemType::Update:
				if (index < _updates.size() - 1)
				{
					std::swap(_updates[index], _updates.back());
					UpdateSystemMapIndex(sysType, _updates.size() - 1, index);
				}
				_updates.pop_back();
				break;
			case SystemType::FixedUpdate:
				if (index < _fixeds.size() - 1)
				{
					std::swap(_fixeds[index], _fixeds.back());
					UpdateSystemMapIndex(sysType, _fixeds.size() - 1, index);
				}
				_fixeds.pop_back();
				break;
			case SystemType::Render:
				if (index < _renders.size() - 1)
				{
					std::swap(_renders[index], _renders.back());
					UpdateSystemMapIndex(sysType, _renders.size() - 1, index);
				}
				_renders.pop_back();
				break;
			}
		}

		if(_systemMap.contains(name))
			_systemMap.erase(name);
	}
}


