#pragma once
#include <physx/PxPhysicsAPI.h>

#define IS_VALID_CONVERT(FROM, TO) (std::is_same_v<FromType, FROM> && std::is_same_v<ToType, TO>)

namespace core
{
	class Scene;
	class Entity;
	struct ColliderCommon;
	struct Rigidbody;

	class PhysicsScene
	{
	public:
		PhysicsScene(Scene& scene);
		~PhysicsScene();

		void Update(float tick);

		bool CreatePhysicsActor(const Entity& entity);
		bool DestroyPhysicsActor(const Entity& entity);

		static void AddLayer(entt::id_type layerId);
		static void SetLayerCollision(entt::id_type layerAId, entt::id_type layerBId, bool canCollide);
		static bool CanLayersCollide(entt::id_type layerAId, entt::id_type layerBId);

		void Clear();

	private:
		void sceneFetch(const Entity& entity);

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

		inline static std::map<entt::id_type, uint32_t> _layerToIndexMap;
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

