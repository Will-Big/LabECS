#include "pch.h"
#include "Systems.h"

#include "Components.h"

void core::TransformSystem::operator()(entt::registry& registry, float tick)
{
	auto view = registry.view<Transform>();
}

void core::PhysicsSystem::operator()(entt::registry& registry, float tick)
{

}

void core::AnimationSystem::operator()(entt::registry& registry, float tick)
{
}

void core::AnimationSystem::operator()(entt::registry& registry, Graphics& graphics, float tick)
{

}
