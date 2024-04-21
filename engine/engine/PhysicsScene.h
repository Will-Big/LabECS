#pragma once
#include <physx/PxPhysicsAPI.h>

#define IS_VALID_CONVERT(FROM, TO) (std::is_same_v<FromType, FROM> && std::is_same_v<ToType, TO>)

namespace core
{
	class Scene;
	class Entity;
	struct ColliderCommon;
	struct Rigidbody;

	/**
	 * \brief
	 * PhysicsScene 에서 정상적으로 엔티티가 동작하기 위한 조건
	 *	- 필수: Transform, ColliderCommon, ~Collider(Box, Sphere, Capsule, Mesh)
	 *	- 옵션: Rigidbody
	 *
	 * 사용 시 세부 설정 가능 (미사용 시 기본으로 사용됨)
	 *	- Tag: 충돌을 처리하는 시스템 정의 가능
	 *		- 기본: 충돌을 처리하는 시스템 없음(tag::Untagged)
	 *	- Layer: 특정 레이어 간 충돌 판정 여부를 설정 가능
	 *		- 기본: 모든 레이어와 충돌(layer::Default)
	 */
	class PhysicsScene
	{
	public:
		PhysicsScene(Scene& scene);
		~PhysicsScene();

		void Update(float tick);

		// 액터 생성/삭제
		void CreatePhysicsActor(const Entity& entity);
		void DestroyPhysicsActor(const Entity& entity);

		// 충돌 매트릭스 설정
		static void InitializeCollisionMatrix() { _collisionMatrix.resize(32); }
		static void AddLayer(entt::id_type layerId);
		static void SetLayerCollision(entt::id_type layerId1, entt::id_type layerId2, bool canCollide);

		void Clear();

	private:
		void sceneFetch(const Entity& entity, const physx::PxActor* actor);

		template <typename ToType, typename FromType>
		ToType convert(const FromType& fromType);

		// 좌표계 변환
		physx::PxTransform rightToLeft(const physx::PxTransform& rightHandTransform);
		physx::PxTransform leftToRight(const physx::PxTransform& leftHandTransform);

		static physx::PxFilterFlags customFilterShader(
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);



	private:
		Scene* _scene = nullptr;
		physx::PxScene* _pxScene = nullptr;

		inline static physx::PxPhysics* _physics = nullptr;
		inline static physx::PxPvd* _pvd = nullptr;
		inline static physx::PxFoundation* _foundation = nullptr;
		inline static physx::PxDefaultAllocator _allocator;
		inline static physx::PxDefaultErrorCallback _errorCallback;

		inline static std::map<entt::id_type, uint32_t> _layerIdToIndexMap;
		inline static std::vector<uint32_t> _collisionMatrix{ 32, 0 }; // 충돌 매트릭스 (32 * 32)

		std::unordered_map<entt::entity, physx::PxActor*> _entityToPxActorMap;
	};

	template <typename ToType, typename FromType>
	ToType PhysicsScene::convert(const FromType& fromType)
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

