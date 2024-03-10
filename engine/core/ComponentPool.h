#pragma once

namespace core
{
    // Component 객체를 소유하고 있는 클래스
    template<typename Component>
    class ComponentPool
	{
    public:
        // Component 를 추가하고 그 인덱스를 반환합니다.
        size_t AddComponent(EntityId entityId, Component&& component = {})
    	{
            std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

            // EntityId 에 해당하는 Component 가 이미 존재하는지 확인
            if (entityToIndex_.contains(entityId)) {
                // 이미 존재하면 기존 Component 를 업데이트
                size_t index = entityToIndex_[entityId];
                components_[index] = std::forward<Component>(component);
                return index;
            }

            // 새 Component 를 추가
            const size_t newIndex = components_.size();
            components_.push_back(std::forward<Component>(component));
            entityToIndex_[entityId] = newIndex;
            indexToEntity_[newIndex] = entityId;
            return newIndex;
        }

        // 주어진 EntityId 의 Component 를 제거합니다.
        void RemoveComponent(EntityId entityId)
    	{
            std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

            assert(entityToIndex_.find(entityId) != entityToIndex_.end());

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

        // 주어진 EntityId 에 해당하는 Component 를 반환합니다. (쓰기 전용)
        // 개별 스레드에서 동시에 Component 를 수정할 경우 문제가 야기될 수 있음.
        Component& GetComponent(EntityId entityId)
        {
            assert(!entityToIndex_.contains(entityId));

            return components_[entityToIndex_[entityId]];
        }

        // 주어진 EntityId 에 해당하는 Component 의 스냅샷을 반환합니다. (읽기 전용)
        const Component& GetSnapShot(EntityId entityId)
    	{
            assert(!entityToIndex_.contains(entityId));

            return components_[entityToIndex_[entityId]];
        }

        // EntityId 로 Component 의 존재 여부를 확인합니다.
        bool HasComponent(EntityId entityId) const
    	{
            return entityToIndex_.find(entityId) != entityToIndex_.end();
        }

        // ComponentPool 을 초기화합니다.
        void Clear()
        {
            std::unique_lock lock(mutex_); // 쓰기 작업을 위한 독점 락

            components_.clear();
            entityToIndex_.clear();
            indexToEntity_.clear();
        }

    private:
        std::shared_mutex mutex_;
        std::vector<Component> components_;
        std::unordered_map<EntityId, size_t> entityToIndex_;
        std::unordered_map<size_t, EntityId> indexToEntity_;
    };
}