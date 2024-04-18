#include "pch.h"
#include "CollisionCallback.h"

#include "Scene.h"
#include "CoreTags.h"
#include "SystemInterface.h"
#include "CoreSystemEvents.h"

#include <physx/PxActor.h>



core::CollisionCallback::CollisionCallback(Scene& scene)
	: _scene(&scene)
{
	auto dispatcher = _scene->GetDispatcher();

	// 콜리전 핸들러 시스템 등록 이벤트 연결
	dispatcher->sink<OnRegisterCollisionHandler>().connect<&CollisionCallback::registerCollisionHandler>(this);
}

void core::CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		// 충돌 시작 이벤트 처리
		processCollisionEvent(cp, pairHeader, physx::PxPairFlag::eNOTIFY_TOUCH_FOUND, &ICollisionHandler::OnCollisionEnter);

		// 충돌 지속 이벤트 처리
		processCollisionEvent(cp, pairHeader, physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS, &ICollisionHandler::OnCollisionStay);

		// 충돌 종료 이벤트 처리
		processCollisionEvent(cp, pairHeader, physx::PxPairFlag::eNOTIFY_TOUCH_LOST, &ICollisionHandler::OnCollisionExit);
	}
}

void core::CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	int k = 0;
}

void core::CollisionCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void core::CollisionCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}

void core::CollisionCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void core::CollisionCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
}

void core::CollisionCallback::processCollisionEvent(const physx::PxContactPair& cp,
	const physx::PxContactPairHeader& pairHeader, physx::PxPairFlag::Enum eventFlag,
	void(ICollisionHandler::* callback)(const Entity&, const Entity&))
{
	if (cp.events & eventFlag)
	{
		auto registry = _scene->GetRegistry();

		entt::entity entityA = { *static_cast<entt::entity*>(pairHeader.actors[0]->userData) };
		entt::entity entityB = { *static_cast<entt::entity*>(pairHeader.actors[1]->userData) };

		auto processEntity = [&](entt::entity self, entt::entity other)
			{
				if (registry->all_of<Tag>(self))
				{
					const auto& tag = registry->get<Tag>(self);

					for (auto it = _handlers.find(tag.id); it != _handlers.end(); ++it)
					{
						(it->second->*callback)({ self, *registry }, { other, *registry });
					}
				}
			};

		processEntity(entityA, entityB);
		processEntity(entityB, entityA);
	}
}

void core::CollisionCallback::registerCollisionHandler(const OnRegisterCollisionHandler& event)
{
	// 특정 타입의 충돌을 관리하는 핸들러 등록 (중첩 가능)
	_handlers.insert({ event.id, event.handler });
}
