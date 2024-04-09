#include "pch.h"
#include "Systems.h"

#include "Components.h"
#include "PhysicsScene.h"

void core::TransformSystem::operator()(entt::registry& registry, float tick)
{
	auto view = registry.view<Transform>();
}

void core::PhysicsSystem::operator()(entt::registry& registry, float tick)
{
	auto rigidView = registry.view<Transform, Rigidbody>();
	auto colliderView = registry.view<ColliderCommon>();

	for(auto&& [a, b, c] : rigidView.each())
	{

	}

	physicsScene->Update(tick);
}

void core::AnimationSystem::operator()(entt::registry& registry, float tick)
{
}

void core::AnimationSystem::operator()(entt::registry& registry, Graphics& graphics, float tick)
{

}
