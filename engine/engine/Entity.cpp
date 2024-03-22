#include "pch.h"
#include "Entity.h"
#include "Components.h"

void engine::Entity::SetParent(Entity entity)
{
	if (!entity.AnyOf<Parent>())
	{
		Parent& parent = this->Emplace<Parent>();
		parent.parentHandle = entity;
	}
	else
	{
		Parent& parent = this->Get<Parent>();
		parent.parentHandle = entity;
	}
}

bool engine::Entity::IsAncestorOf(Entity entity)
{

}
