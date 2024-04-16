#include "pch.h"
#include "CollisionCallback.h"

void core::CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    for (physx::PxU32 i = 0; i < nbPairs; i++) 
    {
        const physx::PxContactPair& cp = pairs[i];

        if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) 
        {
            // 충돌 시작 이벤트 처리
        }
        if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) 
        {
            int k = 0;
			// 충돌 지속 이벤트 처리
        }
        if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) 
        {
            int k = 0;
            // 충돌 종료 이벤트 처리
        }
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
