﻿#include "pch.h"
#include "Entity.h"
#include "CoreComponents.h"

void core::Entity::SetParent(const core::Entity& entity)
{
	if (!entity.HasAnyOf<Relationship>())
	{
		Relationship& parent = this->Emplace<Relationship>();
		parent.parent = entity;
	}
	else
	{
		Relationship& parent = this->Get<Relationship>();
		parent.parent = entity;
	}
}

std::vector<core::Entity> core::Entity::GetChildren() const
{
	auto view = _registry.view<Relationship>();
	std::vector<Entity> children;

	view.each([&](const auto entity, const Relationship& parent)
		{
			if (*this == parent.parent)
				children.emplace_back(entity, _registry);
		});

	return children;
}

bool core::Entity::IsAncestorOf(core::Entity entity) const
{
	while (_registry.any_of<Relationship>(entity))
	{
		const auto& parent = _registry.get<Relationship>(entity);

		if (parent.parent == *this)
			return true;

		entity._handle = parent.parent;
	}

	return false;
}

bool core::Entity::IsDescendantOf(const core::Entity& entity) const
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
