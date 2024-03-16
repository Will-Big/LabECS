#pragma once

#include <typeindex>

#include "ComponentPool.h"

namespace core
{
    // ComponentPool 을 관리하는 클래스
    // 사용자는 ComponentPool 에 직접 접근하지 않으며 PoolManager 를 통해 관리
    class CORE_EXPORT PoolManager
	{
    public:
        // ComponentPool 을 가져옵니다.
        template<typename Component>
        ComponentPool<Component>& GetPool();

        // Entity 에 Component 를 추가하고 인덱스를 반환합니다.
        template<typename Component>
        void AddComponent(EntityId entityId);

        // Entity 에 Component 를 제거합니다.
        template<typename Component>
        void RemoveComponent(EntityId entityId);

        // Entity 에 Component 를 제거합니다.
        void RemoveComponent(ComponentId componentId, EntityId entityId);

        // Entity 에 쓰기 전용 Component 를 가져옵니다.
        template<typename Component>
        Component& GetComponent(EntityId entityId);

        // Entity 에 읽기 전용 Component 를 가져옵니다.
        template<typename Component>
        const Component& GetSnapShot(EntityId entityId);

        // Entity 에 Component 존재 여부를 확인합니다.
        template<typename Component>
        bool HasComponent(EntityId entityId);

    private:
        // ComponentId 를 키로 하여 해당 타입의 ComponentPool 을 저장하는 맵
        std::unordered_map<ComponentId, std::shared_ptr<IComponentPoolBase>> pools_;

        friend class EntityManager;
    };

    template <typename Component>
    ComponentPool<Component>& PoolManager::GetPool()
    {
        constexpr ComponentId componentId = Component::componentId;

	    if (!pools_.contains(componentId)) 
	    {
		    // 해당 타입의 ComponentPool 이 존재하지 않으면 생성
		    auto pool = std::make_shared<ComponentPool<Component>>();
		    pools_[componentId] = pool;
	    }

	    return *std::static_pointer_cast<ComponentPool<Component>>(pools_[componentId]);
    }

    template <typename Component>
    void PoolManager::AddComponent(EntityId entityId)
    {
	    GetPool<Component>().AddComponent(entityId);
    }

    template <typename Component>
    void PoolManager::RemoveComponent(EntityId entityId)
    {
	    GetPool<Component>().RemoveComponent(entityId);
    }

    template <typename Component>
    Component& PoolManager::GetComponent(EntityId entityId)
    {
	    return GetPool<Component>().GetComponent(entityId);
    }

    template <typename Component>
    const Component& PoolManager::GetSnapShot(EntityId entityId)
    {
	    return GetPool<Component>().GetSnapShot(entityId);
    }

    template <typename Component>
    bool PoolManager::HasComponent(EntityId entityId)
    {
	    return GetPool<Component>().HasComponent(entityId);
    }
}
