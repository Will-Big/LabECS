#include "pch.h"
#include "Systems.h"

#include "Scene.h"
#include "Components.h"
#include "PhysicsScene.h"

#pragma region TransformSystem
void core::TransformSystem::operator()(entt::registry& registry, float tick)
{
	auto view = registry.view<Transform>();
}
#pragma endregion

#pragma region PhysicsSystem
core::PhysicsSystem::PhysicsSystem(entt::dispatcher& dispatcher)
	: ISystemBase(dispatcher)
{
	_dispatcher->sink<struct OnStartSystem>().connect<&PhysicsSystem::OnStartSystem>(this);
	_dispatcher->sink<struct OnFinishSystem>().connect<&PhysicsSystem::OnFinishSystem>(this);

	_dispatcher->sink<struct OnStartEntity>().connect<&PhysicsSystem::OnStartEntity>(this);
	_dispatcher->sink<struct OnDestroyEntity>().connect<&PhysicsSystem::OnDestroyEntity>(this);
}

void core::PhysicsSystem::OnStartSystem(const core::OnStartSystem& event)
{
	_physicsScene = event.scene->GetPhysicsScene();

	auto registry = event.scene->GetRegistry();

	for (auto&& [entity, collider] : registry->view<ColliderCommon>().each())
	{
		_physicsScene->AddPhysicsActor({ entity, *registry });
	}
}

void core::PhysicsSystem::OnFinishSystem(const core::OnFinishSystem& event)
{
	_physicsScene = nullptr;
}

void core::PhysicsSystem::OnStartEntity(const core::OnStartEntity& event)
{
	_physicsScene->AddPhysicsActor(*event.entity);
}

void core::PhysicsSystem::OnDestroyEntity(const core::OnDestroyEntity& event)
{
	// todo
}

void core::PhysicsSystem::operator()(entt::registry& registry, float tick)
{
	_physicsScene->Update(tick);

}
#pragma endregion

#pragma region AnimationSystem
void core::AnimationSystem::operator()(entt::registry& registry, float tick)
{
}

void core::AnimationSystem::operator()(entt::registry& registry, Graphics& graphics, float tick)
{

}
#pragma endregion

#pragma region EventTestSystem
core::EventTestSystem::EventTestSystem(entt::dispatcher& dispatcher)
	: ISystemBase(dispatcher)
{
	_dispatcher->sink<struct OnStartSystem>().connect<&EventTestSystem::OnStartSystem>(this);
	_dispatcher->sink<struct OnFinishSystem>().connect<&EventTestSystem::OnFinishSystem>(this);
}

void core::EventTestSystem::OnStartSystem(const core::OnStartSystem& event)
{
}

void core::EventTestSystem::OnFinishSystem(const core::OnFinishSystem& event)
{
}
#pragma endregion
