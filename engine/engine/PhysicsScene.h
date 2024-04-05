#pragma once
#include <physx/PxPhysicsAPI.h>

namespace core
{
	class Entity;
	struct Collider;
	struct Rigidbody;

	class PhysicsScene
	{
	public:
		PhysicsScene();
		~PhysicsScene();

		void Update(float tick);

		void createStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);

		void AddRigidbody(Entity& entity, const Rigidbody& rigidbody);
		void AddCollider(Entity& entity, const Collider& collider);

	private:
		physx::PxScene* _scene = nullptr;
		physx::PxFoundation* _foundation = nullptr;
		physx::PxPhysics* _physics = nullptr;
		physx::PxPvd* _pvd = nullptr;
	};
}

