#include "pch.h"
#include "PhysicsScene.h"

#include "Entity.h"
#include "Components.h"
#include "ComponentInlines.h"

core::PhysicsScene::PhysicsScene()
{
	using namespace physx;

	// PxFoundation 생성
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// PxPvd 생성
	_pvd = PxCreatePvd(*_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	// PxPhysics 생성
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), false, _pvd);

	// PxScene 생성
	PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	PxDefaultCpuDispatcher* dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	_scene = _physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

core::PhysicsScene::~PhysicsScene()
{
	using namespace physx;
}

void core::PhysicsScene::Update(float tick)
{
	// 시뮬레이션 업데이트
	_scene->simulate(tick);
	_scene->fetchResults(true);
}

bool core::PhysicsScene::AddPhysicsActor(const core::Entity& entity)
{
	using namespace physx;

	if (!entity.HasAllOf<Transform, ColliderCommon>())
		return false;

	if (!entity.HasAnyOf<BoxCollider, SphereCollider, CapsuleCollider, MeshCollider>())
		return false;

	const auto& collider = entity.Get<ColliderCommon>();
	const auto& transform = entity.Get<Transform>();

	PxMaterial* material = _physics->createMaterial(
		collider.material.staticFriction,
		collider.material.dynamicFriction,
		collider.material.bounciness);

	PxShape* shape = nullptr;
	PxActor* actor = nullptr;

	switch (collider.shape)
	{
	case ColliderCommon::Shape::Box:
	{
		const auto& box = entity.Get<BoxCollider>();
		shape = _physics->createShape(PxBoxGeometry(box.size.x / 2, box.size.y / 2, box.size.z / 2), *material);
		break;
	}
	case ColliderCommon::Shape::Sphere:
	{
		const auto& sphere = entity.Get<SphereCollider>();
		shape = _physics->createShape(PxSphereGeometry(sphere.radius), *material);
		break;
	}
	case ColliderCommon::Shape::Capsule:
	{
		const auto& capsule = entity.Get<CapsuleCollider>();
		shape = _physics->createShape(PxCapsuleGeometry(capsule.radius, capsule.height), *material);
		break;
	}
	case ColliderCommon::Shape::Mesh:
	{
		/*const auto& mesh = entity.Get<MeshCollider>();
		shape = _physics->createShape(PxConvexMeshGeometry(1.0f, 1.0f, 1.0f), *material);*/
		break;
	}
	case ColliderCommon::Shape::None:
	default:
		material->release();
		return false;
	}

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !collider.isTrigger);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, collider.isTrigger);

	if (entity.HasAllOf<Rigidbody>())
	{
		const auto& rigid = entity.Get<Rigidbody>();

		// 동적 액터 생성
		PxRigidDynamic* dynamicActor = _physics->createRigidDynamic(
			PxTransform(
				PxVec3(transform.position.x, transform.position.y, transform.position.z),
				PxQuat(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w)
			)
		);

		dynamicActor->setMass(rigid.mass);
		dynamicActor->setLinearDamping(rigid.drag);
		dynamicActor->setAngularDamping(rigid.angularDrag);
		dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !rigid.useGravity);
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, rigid.isKinematic);
		dynamicActor->attachShape(*shape);

		auto setLockFlag = [&](Rigidbody::Constraints flag, PxRigidDynamicLockFlag::Enum pxFlag)
			{
				if ((rigid.constraints & flag) != Rigidbody::Constraints::None)
					dynamicActor->setRigidDynamicLockFlag(pxFlag, true);
			};

		setLockFlag(Rigidbody::Constraints::FreezePositionX, PxRigidDynamicLockFlag::eLOCK_LINEAR_X);
		setLockFlag(Rigidbody::Constraints::FreezePositionY, PxRigidDynamicLockFlag::eLOCK_LINEAR_Y);
		setLockFlag(Rigidbody::Constraints::FreezePositionZ, PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
		setLockFlag(Rigidbody::Constraints::FreezeRotationX, PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
		setLockFlag(Rigidbody::Constraints::FreezeRotationY, PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
		setLockFlag(Rigidbody::Constraints::FreezeRotationZ, PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

		actor = dynamicActor;
	}
	else
	{
		// 정적 액터 생성
		PxRigidStatic* staticActor = _physics->createRigidStatic(
			PxTransform(
				PxVec3(transform.position.x, transform.position.y, transform.position.z),
				PxQuat(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w)
			)
		);

		staticActor->attachShape(*shape);

		actor = staticActor;
	}

	// 매핑 테이블에 액터 추가
	_entityToPxActorMap[entity] = actor;
	_scene->addActor(*actor);

	shape->release();
	material->release();

	return true;
}
