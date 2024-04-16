#pragma once
#include <physx/PxPhysicsAPI.h>

#define IS_VALID_CONVERT(FROM, TO) (std::is_same_v<FromType, FROM> && std::is_same_v<ToType, TO>)

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
		void Fetch(const Entity& entity);

		template <typename ToType, typename FromType>
		ToType Convert(const FromType& fromType);

		// 좌표계 변환
		physx::PxTransform RightToLeft(const physx::PxTransform& rightHandTransform);
		physx::PxTransform LeftToRight(const physx::PxTransform& leftHandTransform);

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

	template <typename ToType, typename FromType>
	ToType PhysicsScene::Convert(const FromType& fromType)
	{
		using namespace physx;

		if constexpr (IS_VALID_CONVERT(PxVec3, Vector3)) {
			return Vector3{ fromType.x, fromType.y, fromType.z };
		}
		else if constexpr (IS_VALID_CONVERT(PxVec2, Vector2)) {
			return Vector2{ fromType.x, fromType.y };
		}
		else if constexpr (IS_VALID_CONVERT(PxQuat, Quaternion)) {
			return Quaternion{ fromType.x, fromType.y, fromType.z, fromType.w };
		}
		else if constexpr (IS_VALID_CONVERT(Vector3, PxVec3)) {
			return PxVec3(fromType.x, fromType.y, fromType.z);
		}
		else if constexpr (IS_VALID_CONVERT(Vector2, PxVec2)) {
			return PxVec2(fromType.x, fromType.y);
		}
		else if constexpr (IS_VALID_CONVERT(Quaternion, PxQuat)) {
			return PxQuat(fromType.x, fromType.y, fromType.z, fromType.w);
		}
		else {
			static_assert(IS_VALID_CONVERT(ToType, FromType), "Unsupported type conversion requested.");
			return ToType{};
		}
	}
}

