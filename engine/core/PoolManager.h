#pragma once

#include "ComponentPool.h"

namespace core
{
    // ComponentPool 을 관리하는 클래스
    // 사용자는 ComponentPool 에 직접 접근하지 않으며 PoolManager 를 통해 관리
    // Component 의 추가, 삭제, 읽기/쓰기, 순회를 담당
    class CORE_EXPORT PoolManager
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
        EntityId GetEntityId(const typename Component::Iterator& iterator);

        template<IsComponent Component>
        typename Component::Iterator begin() { return GetPool<Component>().begin(); }

        template<IsComponent Component>
        typename Component::Iterator end() { return GetPool<Component>().end(); }

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
    Component& PoolManager::AddComponent(EntityId entityId)
    {
        return *static_cast<Component*>(GetPool<Component>().AddComponent(entityId));
    }

    template <IsComponent Component>
    void PoolManager::RemoveComponent(EntityId entityId)
    {
	    GetPool<Component>().RemoveComponent(entityId);
    }


    inline void PoolManager::RemoveEntityComponents(EntityId entityId)
    {
        // 모든 ComponentPool 을 순회하며 Entity 에 속한 Component 객체 삭제
        for(const auto& poolBase : std::views::values(pools_))
        {
            poolBase->RemoveComponent(entityId);
        }
    }

    inline void PoolManager::Clear()
    {
        for (const auto& poolBase : std::views::values(pools_))
        {
            poolBase->Clear();
        }
    }

    template <IsComponent Component>
    Component& PoolManager::GetComponent(EntityId entityId)
    {
	    return GetPool<Component>().GetComponent(entityId);
    }

    template <IsComponent Component>
    const Component& PoolManager::GetSnapShot(EntityId entityId)
    {
	    return GetPool<Component>().GetSnapShot(entityId);
    }

    template <IsComponent Component>
    bool PoolManager::HasComponent(EntityId entityId)
    {
	    return GetPool<Component>().HasComponent(entityId);
    }

    template <IsComponent Component>
    EntityId PoolManager::GetEntityId(const typename Component::Iterator& iterator)
    {
        return GetPool<Component>().GetEntityId(iterator);
    }

    template <IsComponent Component>
    ComponentPool<Component>& PoolManager::GetPool()
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
