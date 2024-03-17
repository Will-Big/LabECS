#include "pch.h"
#include "EntityManager.h"

#include "ComponentManager.h"
#include "Parent.h"

core::EntityId core::EntityManager::AddEntity()
{
	return entities_.emplace_back(++lastId_);
}

void core::EntityManager::RemoveEntity(EntityId entityId, ComponentManager* poolManager)
{
	std::lock_guard lock(mutex_);

	std::vector<EntityId> toRemove{ entityId };

	// 첫 번째 Entity 의 모든 자식을 찾습니다.
	std::vector<EntityId> children = GetChildren(entityId, poolManager);

	// 모든 자식 Entity 에 대해 반복
	while (!children.empty())
	{
		std::vector<EntityId> nextChildren;

		// 각 자식에 대해 더 하위의 자식이 있는지 탐색
		for (auto& child : children)
		{
			auto subChildren = GetChildren(child, poolManager);
			nextChildren.insert(nextChildren.end(), subChildren.begin(), subChildren.end());
			toRemove.push_back(child); // 삭제 목록에 추가
		}

		// 다음 레벨의 자식으로 이동
		children = std::move(nextChildren);
	}

	// 모든 엔티티를 삭제
	for (const auto& id : toRemove) 
	{
		// 해당 Entity 와 관련된 모든 컴포넌트를 삭제합니다.
		poolManager->RemoveEntityComponents(id);
	}

	// Entity 목록에서 제거
	entities_.remove_if([&toRemove](const EntityId& id)
		{
			return std::ranges::find(toRemove, id) != toRemove.end();
		});
}

core::EntityId core::EntityManager::GetParent(EntityId childId, ComponentManager* poolManager)
{
	return poolManager->GetPool<Parent>().GetSnapShot(childId).parentId;
}

std::vector<core::EntityId> core::EntityManager::GetChildren(EntityId parentId, ComponentManager* poolManager)
{
	auto& parentPool = poolManager->GetPool<Parent>();
	std::vector<core::EntityId> children;

	for (auto it = parentPool.begin(); it != parentPool.end(); ++it)
	{
		if (it->parentId == parentId)
			children.push_back(parentPool.GetEntityId(it));
	}

	return children;
}

void core::EntityManager::Clear()
{
	entities_.clear();
	lastId_ = 0;
}
