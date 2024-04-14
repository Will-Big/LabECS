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
		PhysicsScene(entt::dispatcher& dispatcher);
		~PhysicsScene();

		void Update(float tick);

		bool CreatePhysicsActor(const Entity& entity);
		bool DestroyPhysicsActor(const Entity& entity);

		void Clear();

	private:
		entt::dispatcher* _dispatcher = nullptr;
		physx::PxScene* _scene = nullptr;

		inline static physx::PxPhysics* _physics = nullptr;
		inline static physx::PxPvd* _pvd = nullptr;
		inline static physx::PxFoundation* _foundation = nullptr;
		inline static physx::PxDefaultAllocator _allocator;
		inline static physx::PxDefaultErrorCallback _errorCallback;

		std::unordered_map<entt::entity, physx::PxActor*> _entityToPxActorMap;
	};
}

