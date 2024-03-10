#include "pch.h"
#include "EntityManager.h"

core::EntityId core::EntityManager::AddEntity()
{
	return entities_.emplace_back(++lastId_);
}

void core::EntityManager::RemoveEntity(EntityId entityId)
{
	std::lock_guard lock(mutex_);

	entities_.remove(entityId);
}

void core::EntityManager::Clear()
{
	entities_.clear();
	lastId_ = 0;
}
