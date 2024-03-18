#pragma once

namespace core
{
	class ComponentManager;

	// Entity 를 관리하는 클래스
	// Entity 의 추가, 삭제, 계층 구조를 담당
	class CORE_API EntityManager
	{
	public:
		// Entity 를 추가하고 EntityId 를 반환합니다.
		EntityId AddEntity();

		// Entity 와 하위 Entity 에 속한 모든 Component 를 제거한 후 Entity 를 제거합니다.
		void RemoveEntity(EntityId entityId, ComponentManager* poolManager);

		// chlidId 의 부모 EntityId 를 반환합니다.
		EntityId GetParent(EntityId childId, ComponentManager* poolManager);

		// parentId 의 자식 EntityId 를 모두 반환합니다.
		std::vector<EntityId> GetChildren(EntityId parentId, ComponentManager* poolManager);

		// 모든 Entity 를 제거하고 lastId_ 를 초기화합니다.
		void Clear();

	private:
		std::mutex mutex_;
		std::list<EntityId> entities_;
		std::atomic<EntityId> lastId_ = 0;
	};
}
