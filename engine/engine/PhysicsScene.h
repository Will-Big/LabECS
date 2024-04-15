#pragma once
#include <physx/PxPhysicsAPI.h>

#define PX_TO_CORE_VALID(CORE, PHYSX) (std::is_same_v<CoreType, CORE> && std::is_same_v<PhysxType, PHYSX>)

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

		void Fetch(const Entity& entity);

		template <typename CoreType, typename PhysxType>
		CoreType Convert(const PhysxType& physxType);

		physx::PxTransform RightToLeft(const physx::PxTransform& rightHandTransform);
		physx::PxTransform LeftToRight(const physx::PxTransform& leftHandTransform);

		void Clear();

		static physx::PxFilterFlags CustomFilterShader(
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

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

	template <typename CoreType, typename PhysxType>
	CoreType PhysicsScene::Convert(const PhysxType& physxType)
	{
		static_assert(PX_TO_CORE_VALID(CoreType, PhysxType), "Unsupported type conversion requested.");

		using namespace physx;

		if constexpr (PX_TO_CORE_VALID(Vector2, PxVec2))
		{
			return Vector3{ physxType.x, physxType.y };
		}
		if constexpr (PX_TO_CORE_VALID(Vector3, PxVec3))
		{
			return Vector3{ physxType.x, physxType.y, physxType.z };
		}
		else if constexpr (PX_TO_CORE_VALID(Quaternion, PxQuat))
		{
			return Quaternion{ physxType.x, physxType.y, physxType.z, physxType.w };
		}
		else
		{
			return CoreType{};
		}
	}
}

