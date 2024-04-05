#pragma once
#include <iostream>

namespace core
{
	struct Transform
	{
		float x;
		float y;
		float z;
	};

	struct Relationship
	{
		entt::entity parent;
	};

	struct Name
	{
		std::string name;
	};

	struct Rigidbody
	{
		enum class Interpolation : uint_fast8_t
		{
			None,
			Interpolate,
			Extrapolate
		};
		enum class Constraints : uint_fast8_t
		{
			None = 0,
			FreezePositionX = 1 << 0, // 1
			FreezePositionY = 1 << 1, // 2
			FreezePositionZ = 1 << 2, // 4
			FreezeRotationX = 1 << 3, // 8
			FreezeRotationY = 1 << 4, // 16
			FreezeRotationZ = 1 << 5, // 32
			FreezePosition = FreezePositionX | FreezePositionY | FreezePositionZ,	// 7
			FreezeRotation = FreezeRotationX | FreezeRotationY | FreezeRotationZ,	// 56
			FreezeAll = FreezePosition | FreezeRotation,							// 63
		};

		float mass = 0.f;
		float drag = 0.f;
		float angularDrag = 0.f;
		bool useGravity = false;
		bool isKinematic = false;
		Interpolation interpolation = Interpolation::None;
		Constraints constraints = Constraints::None;
	};

	struct Collider
	{
		struct PhysicMaterial
		{
			float dynamicFriction = 0.f;
			float staticFriction = 0.f;
			float bounciness = 0.f;
		};

		bool isTrigger;
		PhysicMaterial material;
	};

	struct BoxCollider : Collider
	{
		
	};

	struct SphereCollider : Collider
	{
		
	};

	struct CapsuleCollider : Collider
	{
		
	};

	struct MeshCollider : Collider
	{
		
	};
}




