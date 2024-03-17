#pragma once

#include "ComponentPool.h"

namespace core
{
	// ComponentPool 을 관리하는 클래스
	// 사용자는 ComponentPool 에 직접 접근하지 않으며 ComponentManager 를 통해 관리
	// Component 의 추가, 삭제, 읽기/쓰기, 순회를 담당
	class ComponentManager
	{
	public:
		// Entity 에 Component 를 추가하고 참조를 반환합니다.
		template<IsComponent Component>
		Component& AddComponent(EntityId entityId);

		// Entity 에 Component 를 제거합니다.
		template<IsComponent Component>
		void RemoveComponent(EntityId entityId);

		// Entity 에 속한 모든 Component 를 제거합니다.
		void RemoveEntityComponents(EntityId entityId);

		// 모든 ComponentPool 을 Clear 합니다.
		void Clear();

		// Entity 에 쓰기 전용(참조) Component 를 가져옵니다.
		template<IsComponent Component>
		Component& GetComponent(EntityId entityId);

		// Entity 에 읽기 전용(const 참조) Component 를 가져옵니다.
		template<IsComponent Component>
		const Component& GetSnapShot(EntityId entityId);

		// Entity 에 Component 존재 여부를 확인합니다.
		template<IsComponent Component>
		bool HasComponent(EntityId entityId);

		// Iterator 의 EntityId 를 반환합니다.
		template<IsComponent Component>
		EntityId GetEntityId(const typename ComponentPool<Component>::Iterator& iterator);

		template<IsComponent Component>
		typename ComponentPool<Component>::Iterator begin() { return GetPool<Component>().begin(); }

		template<IsComponent Component>
		typename ComponentPool<Component>::Iterator end() { return GetPool<Component>().end(); }

		// EntityId 에 해당하는 Component 에 람다 함수 적용
		template<typename Component, typename Func>
		void ForEachComponent(Func&& func);

		template<typename... Components>
		std::vector<EntityId> FindCommonEntities(ComponentPool<Components>&... pools)
		{
			// 각 컴포넌트 풀의 entityToIndex_ 맵에서 EntityId 집합을 추출합니다.
			std::vector<std::unordered_set<EntityId>> entitySets;
			(entitySets.emplace_back([&pools] 
				{
				std::unordered_set<EntityId> keys;

				for (const auto& [key, _] : pools.entityToIndex_)
					keys.insert(key);

				return keys;
				}()), ...);

			// 첫 번째 컴포넌트 풀의 EntityId 집합으로 시작하여 공통 EntityId를 찾습니다.
			std::unordered_set<EntityId> commonEntities = entitySets.front();
			for (size_t i = 1; i < entitySets.size(); ++i) {
				std::unordered_set<EntityId> temp;
				std::ranges::set_intersection(commonEntities, entitySets[i],
				                              std::inserter(temp, temp.begin()));
				commonEntities = std::move(temp);
			}

			return std::vector<EntityId>(commonEntities.begin(), commonEntities.end());
		}

		template<typename... Components, typename Func>
		void ForEachCommonComponent(Func&& func)
		{
			auto commonEntityIds = FindCommonEntities(GetPool<Components>()...);

			for (auto entityId : commonEntityIds) {
				func(GetPool<Components>().GetComponent(entityId)...);
			}
		}


	private:
		// ComponentPool 을 가져옵니다.
		template<IsComponent Component>
		ComponentPool<Component>& GetPool();

	private:
		// ComponentId 를 키로 하여 해당 타입의 ComponentPool 을 저장하는 맵
		std::unordered_map<ComponentId, std::shared_ptr<IComponentPoolBase>> pools_;

		friend class EntityManager;
	};

	template <IsComponent Component>
	Component& ComponentManager::AddComponent(EntityId entityId)
	{
		return *static_cast<Component*>(GetPool<Component>().AddComponent(entityId));
	}

	template <IsComponent Component>
	void ComponentManager::RemoveComponent(EntityId entityId)
	{
		GetPool<Component>().RemoveComponent(entityId);
	}


	inline void ComponentManager::RemoveEntityComponents(EntityId entityId)
	{
		// 모든 ComponentPool 을 순회하며 Entity 에 속한 Component 객체 삭제
		for (const auto& poolBase : std::views::values(pools_))
		{
			poolBase->RemoveComponent(entityId);
		}
	}

	inline void ComponentManager::Clear()
	{
		for (const auto& poolBase : std::views::values(pools_))
		{
			poolBase->Clear();
		}
	}

	template <IsComponent Component>
	Component& ComponentManager::GetComponent(EntityId entityId)
	{
		return GetPool<Component>().GetComponent(entityId);
	}

	template <IsComponent Component>
	const Component& ComponentManager::GetSnapShot(EntityId entityId)
	{
		return GetPool<Component>().GetSnapShot(entityId);
	}

	template <IsComponent Component>
	bool ComponentManager::HasComponent(EntityId entityId)
	{
		return GetPool<Component>().HasComponent(entityId);
	}

	template <IsComponent Component>
	EntityId ComponentManager::GetEntityId(const typename ComponentPool<Component>::Iterator& iterator)
	{
		return GetPool<Component>().GetEntityId(iterator);
	}

	template <typename Component, typename Func>
	void ComponentManager::ForEachComponent(Func&& func)
	{
		auto& pool = GetPool<Component>();

		for (auto& component : pool) 
		{
			func(component);
		}
	}

	template <IsComponent Component>
	ComponentPool<Component>& ComponentManager::GetPool()
	{
		// core/Defines.h 참조
		constexpr ComponentId componentId = Component::componentId;

		if (!pools_.contains(componentId))
		{
			// 해당 타입의 ComponentPool 이 존재하지 않으면 생성
			auto pool = std::make_shared<ComponentPool<Component>>();
			pools_[componentId] = pool;
		}

		return *std::static_pointer_cast<ComponentPool<Component>>(pools_[componentId]);
	}
}
