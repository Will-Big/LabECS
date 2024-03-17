#pragma once

namespace core
{
	// ComponentManager 에서 관리하기 위한 인터페이스
	class IComponentPoolBase
	{
	public:
		virtual ~IComponentPoolBase() = default;

		// 주어진 EntityId 의 Component 를 추가하고 포인터를 반환합니다.
		virtual void* AddComponent(EntityId entityId) = 0;

		// 주어진 EntityId 의 Component 를 제거합니다.
		virtual void RemoveComponent(EntityId entityId) = 0;

		// ComponentPool 을 초기화합니다.
		virtual void Clear() = 0;
	};

	// Component 객체를 소유하고 있는 클래스
	template<IsComponent Component>
	class ComponentPool : public IComponentPoolBase
	{
	public:
		class  Iterator
		{
		public:
			using pointer = Component*;
			using reference = Component&;

			Iterator(pointer ptr) : ptr_(ptr) {}

			reference operator*() const { return *ptr_; }
			pointer operator->() { return ptr_; }

			Iterator& operator++() { ++ptr_; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr_ == b.ptr_; }
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr_ != b.ptr_; }

		private:
			pointer ptr_;

			friend class ComponentPool;
		};

		~ComponentPool() override = default;

		// 주어진 EntityId 의 Component 를 추가하고 포인터를 반환합니다.
		void* AddComponent(EntityId entityId) override;

		// 주어진 EntityId 의 Component 를 제거합니다.
		void RemoveComponent(EntityId entityId) override;

		// ComponentPool 을 초기화합니다.
		void Clear() override;

		// 주어진 EntityId 에 해당하는 Component 를 반환합니다. (쓰기 전용)
		// 개별 스레드에서 동시에 Component 를 수정할 경우 문제가 야기될 수 있음.
		Component& GetComponent(EntityId entityId);

		// 주어진 EntityId 에 해당하는 Component 의 스냅샷을 반환합니다. (읽기 전용)
		const Component& GetSnapShot(EntityId entityId);

		// EntityId 로 Component 의 존재 여부를 확인합니다.
		bool HasComponent(EntityId entityId) const;

		// Iterator 의 EntityId 를 반환합니다.
		EntityId GetEntityId(const Iterator& iterator);

		// range-based for loop iterator
		Iterator begin() { return Iterator{ components_.data() }; }
		Iterator end() { return Iterator{ components_.data() + components_.size() }; }

	private:
		std::shared_mutex mutex_;
		std::vector<Component> components_;
		std::unordered_map<EntityId, size_t> entityToIndex_;
		std::unordered_map<size_t, EntityId> indexToEntity_;

		friend class ComponentManager;
	};

	template <IsComponent Component>
	void* ComponentPool<Component>::AddComponent(EntityId entityId)
	{
		// EntityId 에 해당하는 Component 가 이미 존재하는지 확인
		if (entityToIndex_.contains(entityId)) 
		{
			size_t index = entityToIndex_[entityId];
			return &components_[index]; // 기존 인스턴스 반환
		}

		std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

		// 새 Component 를 추가
		const size_t newIndex = components_.size();
		
		entityToIndex_[entityId] = newIndex;
		indexToEntity_[newIndex] = entityId;

		return &components_.emplace_back();
	}

	template <IsComponent Component>
	void ComponentPool<Component>::RemoveComponent(EntityId entityId)
	{
		// EntityId 에 해당하는 Component 가 존재하는지 확인
		if (!entityToIndex_.contains(entityId))
			return;

		std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

		// 마지막 Component 와 삭제할 Component 의 위치를 교환
		size_t indexOfRemoved = entityToIndex_[entityId];
		size_t indexOfLast = components_.size() - 1;
		components_[indexOfRemoved] = components_[indexOfLast];

		// Entity 와 인덱스의 매핑을 업데이트
		const EntityId lastEntity = indexToEntity_[indexOfLast];
		entityToIndex_[lastEntity] = indexOfRemoved;
		indexToEntity_[indexOfRemoved] = lastEntity;

		// 마지막 Component 를 제거하고 매핑을 삭제
		components_.pop_back();
		entityToIndex_.erase(entityId);
		indexToEntity_.erase(indexOfLast);
	}

	template <IsComponent Component>
	Component& ComponentPool<Component>::GetComponent(EntityId entityId)
	{
		assert(entityToIndex_.contains(entityId));

		return components_[entityToIndex_[entityId]];
	}

	template <IsComponent Component>
	const Component& ComponentPool<Component>::GetSnapShot(EntityId entityId)
	{
		assert(entityToIndex_.contains(entityId));

		return components_[entityToIndex_[entityId]];
	}

	template <IsComponent Component>
	bool ComponentPool<Component>::HasComponent(EntityId entityId) const
	{
		return entityToIndex_.contains(entityId);
	}

	template <IsComponent Component>
	EntityId ComponentPool<Component>::GetEntityId(const Iterator& iterator)
	{
		// Iterator 가 가리키는 컴포넌트의 포인터를 얻습니다.
		auto* componentPtr = iterator.ptr_;

		// components_ 벡터의 시작부터 현재 컴포넌트까지의 거리(인덱스)를 계산합니다.
		size_t index = std::distance(components_.data(), componentPtr);

		if(!indexToEntity_.contains(index))
		{
			throw std::runtime_error("EntityId for the given iterator not found.");
		}

		return indexToEntity_[index];
	}



	template <IsComponent Component>
	void ComponentPool<Component>::Clear()
	{
		std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

		components_.clear();
		entityToIndex_.clear();
		indexToEntity_.clear();
	}
}
