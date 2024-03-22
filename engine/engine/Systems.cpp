#include "pch.h"
#include "Systems.h"

#include "Components.h"

void engine::TransformSystem::operator()(entt::registry& registry, float tick)
{
	auto view = registry.view<Transform>();
}
