#pragma once

namespace core
{
	// Entity 를 관리하는 클래스
	// todo: PoolManager 와의 결합도 낮추기
	class EntityManager
	{
	public:
		// Entity 를 추가하고 EntityId 를 반환합니다.
		EntityId AddEntity();

		// Entity 와 하위 Entity 에 속한 Component 를 제거합니다.
		void RemoveEntity(EntityId entityId);

		// 모든 Entity 를 제거하고 lastId_ 를 초기화합니다.
		void Clear();

	private:
		std::mutex mutex_;
		std::list<EntityId> entities_;
		std::atomic<EntityId> lastId_ = 0;
	};
}
