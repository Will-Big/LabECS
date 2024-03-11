#include "pch.h"
#include "EntityManager.h"

#include "PoolManager.h"

core::EntityId CORE_EXPORT core::EntityManager::AddEntity()
{
	return entities_.emplace_back(++lastId_);
}

void core::EntityManager::RemoveEntity(EntityId entityId, PoolManager* poolManager)
{
	std::lock_guard lock(mutex_);

	// 1. 이 Entity 를 부모로 하는 Entity 에 대해 재귀적 삭제

	// 2. 이 Entity 를 주인으로 하는 Component 삭제


	entities_.remove(entityId);

	for (auto& [componentId, pool] : poolManager->pools_)
	{

	}
}

void CORE_EXPORT core::EntityManager::Clear()
{
	entities_.clear();
	lastId_ = 0;
}
