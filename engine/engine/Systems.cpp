#include "pch.h"
#include "Systems.h"

#include "Components.h"

void engine::TransformSystem::operator()(entt::registry& registry, float tick)
{
	auto view = registry.view<Transform>();


}

void engine::PhysicsSystem::operator()(entt::registry& registry, float tick)
{

}

void engine::AnimationRenderSystem::operator()(entt::registry& registry, float tick)
{
}

void engine::AnimationRenderSystem::operator()(entt::registry& registry, Graphics& graphics, float tick)
{

}
