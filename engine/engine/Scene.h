#pragma once
#include "Entity.h"
#include "SystemTraits.h"

namespace engine
{
	class Graphics;

	enum class SystemType
	{
		Update,
		FixedUpdate,
		Render,
	};

	class Scene
	{
		using RenderSystemCallback = std::function<void(entt::registry&, Graphics&, float)>;
		using SystemCallback = std::function<void(entt::registry&, float)>;
		using SystemIndex = std::pair<SystemType, size_t>;

	public:
		Entity AddEntity();

		template <typename T>
		void RegisterSystem();

		template <typename T>
		void RemoveSystem();

		bool Serialize(const std::string& path);
		bool Deserialize(const std::string& path);

		void Run();

	private:
		void UpdateSystemMapIndex(SystemType type, size_t oldIndex, size_t newIndex);

		entt::registry _registry;
		std::unordered_map<std::string, SystemIndex> _systemMap;
		std::vector<SystemCallback> _updateSystems;
		std::vector<SystemCallback> _fixedUpdateSystems;
		std::vector<RenderSystemCallback> _renderSystems;
	};

	template <typename T>
	void Scene::RegisterSystem()
	{
		// 시스템 메타 데이터
		constexpr auto name =  SystemTraits<T>::name;
		constexpr auto type =  SystemTraits<T>::type;

		if (_systemMap.contains(name))
			return;

		if constexpr (type == SystemType::Render)
		{
			// Render 시스템용 람다
			auto systemObj = [system = T{}](entt::registry& registry, Graphics& graphics, float tick) mutable
				{
					system(registry, graphics, tick);
				};

			_renderSystems.push_back(systemObj);
			_systemMap[name] = { type, _renderSystems.size() - 1 };
		}
		else
		{
			// Update 또는 FixedUpdate 시스템용 람다
			auto systemObj = [system = T{}](entt::registry& registry, float tick) mutable
				{
					system(registry, tick);
				};

			// sysType에 따라 적절한 컨테이너 선택 및 추가
			if constexpr (type == SystemType::Update)
			{
				_updateSystems.push_back(systemObj);
				_systemMap[name] = { type, _updateSystems.size() - 1 };
			}
			else if constexpr (type == SystemType::FixedUpdate)
			{
				_fixedUpdateSystems.push_back(systemObj);
				_systemMap[name] = { type, _fixedUpdateSystems.size() - 1 };
			}
		}


	}

	template <typename T>
	void Scene::RemoveSystem()
	{
		// 시스템 메타 데이터
		constexpr auto name = SystemTraits<T>::name;
		constexpr auto type = SystemTraits<T>::type;

		auto it = _systemMap.find(name);

		if (it == _systemMap.end())
			return;

		// 시스템 타입과 인덱스 가져오기
		auto [sysType, index] = it->second;

		// 시스템 타입에 따라 적절한 컨테이너에서 시스템 삭제
		switch (sysType)
		{
		case SystemType::Render:
			if (index < _renderSystems.size() - 1) {
				std::swap(_renderSystems[index], _renderSystems.back());
				UpdateSystemMapIndex(sysType, _renderSystems.size() - 1, index);
			}
			_renderSystems.pop_back();
			break;
		case SystemType::Update:
			if (index < _updateSystems.size() - 1) {
				std::swap(_updateSystems[index], _updateSystems.back());
				UpdateSystemMapIndex(sysType, _updateSystems.size() - 1, index);
			}
			_updateSystems.pop_back();
			break;
		case SystemType::FixedUpdate:
			if (index < _fixedUpdateSystems.size() - 1) {
				std::swap(_fixedUpdateSystems[index], _fixedUpdateSystems.back());
				UpdateSystemMapIndex(sysType, _fixedUpdateSystems.size() - 1, index);
			}
			_fixedUpdateSystems.pop_back();
			break;
		}

		_systemMap.erase(it);
	}
}


