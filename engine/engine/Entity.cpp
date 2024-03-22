#include "pch.h"
#include "Entity.h"
#include "Components.h"

void engine::Entity::SetParent(Entity entity)
{
	if (!entity.HasAnyOf<Parent>())
	{
		Parent& parent = this->Emplace<Parent>();
		parent.handle = entity;
	}
	else
	{
		Parent& parent = this->Get<Parent>();
		parent.handle = entity;
	}
}

std::vector<engine::Entity> engine::Entity::GetChildren()
{
	auto view = _registry.view<Parent>();
	std::vector<Entity> children;

	view.each([&](const auto entity, const Parent& parent)
		{
			if (*this == parent.handle)
				children.emplace_back(entity, _registry);
		});

	return children;
}

bool engine::Entity::IsAncestorOf(Entity entity) const
{
	while (_registry.any_of<Parent>(entity))
	{
		auto parent = _registry.get<Parent>(entity);

		if (parent.handle == *this)
			return true;

		entity._handle = parent.handle;
	}

	return false;
}

bool engine::Entity::IsDescendantOf(Entity entity) const
{
	auto children = entity.GetChildren();

	while(!children.empty())
	{
		std::vector<Entity> descendants;

		for(auto& child : children)
		{
			if (child == this->_handle)
				return true;

			auto childrenOfChild = child.GetChildren();
			descendants.insert(descendants.end(), childrenOfChild.begin(), childrenOfChild.end());
		}

		children = std::move(descendants);
	}

	return false;
}
