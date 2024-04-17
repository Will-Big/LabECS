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
	: ISystem(dispatcher)
{
	_dispatcher->sink<OnStartSystem>().connect<&PhysicsSystem::startSystem>(this);
	_dispatcher->sink<OnFinishSystem>().connect<&PhysicsSystem::finishSystem>(this);

	_dispatcher->sink<OnCreateEntity>().connect<&PhysicsSystem::createEntity>(this);
	_dispatcher->sink<OnDestroyEntity>().connect<&PhysicsSystem::destroyEntity>(this);
}

void core::PhysicsSystem::startSystem(const core::OnStartSystem& event)
{
	_physicsScene = event.scene->GetPhysicsScene();

	auto registry = event.scene->GetRegistry();

	// 씬 시작시 액터 생성
	for (auto&& [entity, collider] : registry->view<ColliderCommon>().each())
	{
		_physicsScene->CreatePhysicsActor({ entity, *registry });
	}
}

void core::PhysicsSystem::finishSystem(const core::OnFinishSystem& event)
{
	auto registry = event.scene->GetRegistry();

	// 씬 종료시 액터 삭제
	for (auto&& [entity, collider] : registry->view<ColliderCommon>().each())
	{
		_physicsScene->DestroyPhysicsActor({ entity, *registry });
	}

	_physicsScene = nullptr;
}

void core::PhysicsSystem::createEntity(const core::OnCreateEntity& event)
{
	// 런타임 중 액터 생성
	_physicsScene->CreatePhysicsActor(event.entity);
}

void core::PhysicsSystem::destroyEntity(const core::OnDestroyEntity& event)
{
	// 런타임 중 액터 삭제
	_physicsScene->DestroyPhysicsActor(event.entity);
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
core::CollisionTesterSystem::CollisionTesterSystem(entt::dispatcher& dispatcher)
	: ISystem(dispatcher)
{
	_dispatcher->sink<OnStartSystem>().connect<&CollisionTesterSystem::startSystem>(this);
	_dispatcher->sink<OnFinishSystem>().connect<&CollisionTesterSystem::finishSystem>(this);
}

void core::CollisionTesterSystem::startSystem(const core::OnStartSystem& event)
{
	auto dispatcher = event.scene->GetDispatcher();

	// 시작시 충돌을 관리할 컴포넌트 타입에 대해 
	dispatcher->trigger<OnRegisterCollisionHandler>({entt::type_hash<TestObject>(), this});
}

void core::CollisionTesterSystem::finishSystem(const core::OnFinishSystem& event)
{
}

void core::CollisionTesterSystem::OnCollisionEnter(const Entity& self, const Entity& other)
{
	int i = 0;
}

void core::CollisionTesterSystem::OnCollisionStay(const Entity& self, const Entity& other)
{
	int i = 0;
}

void core::CollisionTesterSystem::OnCollisionExit(const Entity& self, const Entity& other)
{
	int i = 0;
}
#pragma endregion
