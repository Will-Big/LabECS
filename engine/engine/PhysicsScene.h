#pragma once
#include <physx/PxPhysicsAPI.h>

namespace core
{
	class Entity;
	struct ColliderCommon;
	struct Rigidbody;

	class PhysicsScene
	{
	public:
		PhysicsScene();
		~PhysicsScene();

		void Update(float tick);

		bool AddPhysicsActor(const Entity& entity);

	private:
		physx::PxScene* _scene = nullptr;
		physx::PxFoundation* _foundation = nullptr;
		physx::PxPhysics* _physics = nullptr;
		physx::PxPvd* _pvd = nullptr;

		inline static physx::PxDefaultAllocator _allocator;
		inline static physx::PxDefaultErrorCallback _errorCallback;

		std::unordered_map<entt::entity, physx::PxActor*> _entityToPxActorMap;
	};
}

