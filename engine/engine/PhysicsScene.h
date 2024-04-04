#pragma once
#include <physx/PxPhysicsAPI.h>

namespace core
{
	class PhysicsScene
	{
	public:
		PhysicsScene();
		~PhysicsScene();

		void Update(float tick);

		void createStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);

	private:
		physx::PxScene* _scene = nullptr;
		physx::PxFoundation* _foundation = nullptr;
		physx::PxPhysics* _physics = nullptr;
		physx::PxPvd* _pvd = nullptr;
	};
}

