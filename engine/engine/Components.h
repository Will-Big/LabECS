#pragma once

namespace core
{
	class Mesh;

	struct Transform
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		Matrix localMatrix;
		Matrix worldMatrix;
	};

	struct Relationship
	{
		entt::entity parent = entt::null;
	};

	struct Name
	{
		std::string name;
	};

	struct Rigidbody
	{
		enum class Interpolation
		{
			None,
			Interpolate,
			Extrapolate
		};
		enum class Constraints
		{
			None = 0,
			FreezePositionX = (1 << 0),
			FreezePositionY = (1 << 1),
			FreezePositionZ = (1 << 2),
			FreezeRotationX = (1 << 3),
			FreezeRotationY = (1 << 4),
			FreezeRotationZ = (1 << 5),
			FreezePosition = FreezePositionX | FreezePositionY | FreezePositionZ,
			FreezeRotation = FreezeRotationX | FreezeRotationY | FreezeRotationZ,
			FreezeAll = FreezePosition | FreezeRotation,
		};

		float mass = 0.f;
		float drag = 0.f;
		float angularDrag = 0.f;
		Vector3 velocity;
		bool useGravity = false;
		bool isKinematic = false;
		Interpolation interpolation = Interpolation::None;
		Constraints constraints = Constraints::None;
	};

	struct ColliderCommon
	{
		struct PhysicMaterial
		{
			float dynamicFriction = 0.f;
			float staticFriction = 0.f;
			float bounciness = 0.f;
		};

		bool isTrigger = false;
		PhysicMaterial material;
		std::string sharedMaterial;
	};

	struct BoxCollider
	{
		Vector3 center;
		Vector3 size;
	};

	struct SphereCollider
	{
		Vector3 center;
		float radius = 0.f;
	};

	struct CapsuleCollider
	{
		Vector3 center;
		float radius = 0.f;
		float height = 0.f;
		int direction = 0;
	};

	struct MeshCollider
	{
		enum class MeshColliderCookingOptions
		{
			None,
			CookForFasterSimulation,
			EnableMeshCleaning,
			WeldColocatedVertices,
			UseFastMidphase,
		};

		bool convex = false;
		std::string sharedMesh;
		MeshColliderCookingOptions cookingOptions = MeshColliderCookingOptions::None;
	};
}




