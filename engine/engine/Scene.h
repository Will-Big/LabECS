#pragma once
#include "Entity.h"
#include "SystemTraits.h"

namespace engine
{
	enum class SystemType
	{
		Update,
		FixedUpdate,
		Render,
	};

	class Scene
	{
		using SystemCallback = std::function<void(entt::registry&, float)>;
		using SystemIndex = std::pair<SystemType, size_t>;

	public:
		Entity AddEntity();

		template <typename T> requires IsSystemCallable<T>
		void RegisterSystem();

		template <typename T> requires IsSystemCallable<T>
		void RemoveSystem();

		// todo: Serialize, Deserialize
		bool Serialize(std::filesystem::path& path);
		bool Deserialize(std::filesystem::path& path);

		void Run();

	private:
		std::vector<SystemCallback>& GetSystemContainer(SystemType type);

		entt::registry _registry;
		std::unordered_map<std::string, SystemIndex> _systemMap;
		std::vector<SystemCallback> _updateSystems;
		std::vector<SystemCallback> _fixedUpdateSystems;
		std::vector<SystemCallback> _renderSystems;
	};

	template <typename T> requires IsSystemCallable<T>
	void Scene::RegisterSystem()
	{
		// 시스템의 메타데이터 가져오기
		std::string name = SystemTraits<T>::name;
		SystemType type = SystemTraits<T>::type;

		if (_systemMap.contains(name))
			return;

		// 시스템의 실행 로직을 래핑
		auto systemFunc = [system = T{}](entt::registry& registry, float tick) mutable {
			system(registry, tick);
			};

		// 시스템 유형에 따라 적절한 컨테이너 선택 및 시스템 추가
		auto& systemContainer = GetSystemContainer(type);
		systemContainer.push_back(systemFunc);

		// 시스템 이름과 인덱스를 맵에 저장
		_systemMap[name] = { type, systemContainer.size() - 1 };
	}

	template <typename T> requires IsSystemCallable<T>
	void Scene::RemoveSystem()
	{
		// 시스템의 메타데이터 가져오기
		std::string systemName = SystemTraits<T>::name;
		SystemType systemType = SystemTraits<T>::type;

		auto it = _systemMap.find(systemName);

		if (it == _systemMap.end())
			return;

		auto [type, index] = it->second;
		auto& systemContainer = GetSystemContainer(type);

		// 시스템 컨테이너에서 해당 인덱스의 시스템을 제거합니다.
		// 마지막 요소를 제거할 위치로 이동시킨 후 pop_back을 호출합니다.
		if (index < systemContainer.size() - 1) {
			std::swap(systemContainer[index], systemContainer.back());
		}
		systemContainer.pop_back();

		_systemMap.erase(it);
	}
}


