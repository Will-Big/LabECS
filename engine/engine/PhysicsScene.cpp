#include "pch.h"
#include "PhysicsScene.h"

#include "Entity.h"
#include "Components.h"
#include "ComponentInlines.h"
#include "CollisionCallback.h"

core::PhysicsScene::PhysicsScene(entt::dispatcher& dispatcher)
	: _dispatcher(&dispatcher)
{
	using namespace physx;

	// PxFoundation 생성
	if (!_foundation)
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// PxPvd 생성
	if (!_pvd)
		_pvd = PxCreatePvd(*_foundation);

	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	// PxPhysics 생성
	if (!_physics)
	{
		if (transport && _pvd->connect(*transport, PxPvdInstrumentationFlag::eALL))
			_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), false, _pvd);
		else
			_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale());
	}

	// PxScene 생성
	PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	PxDefaultCpuDispatcher* pxDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = pxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	_scene = _physics->createScene(sceneDesc);
	_scene->setSimulationEventCallback(new CollisionCallback(*_dispatcher));

	if (PxPvdSceneClient* pvdClient = _scene->getScenePvdClient())
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

core::PhysicsScene::~PhysicsScene()
{
	using namespace physx;

	Clear();

	delete _scene->getSimulationEventCallback();
	_scene->release();
	_pvd->release();
	_physics->release();
	_foundation->release();
}

void core::PhysicsScene::Update(float tick)
{
	using namespace physx;

	// 시뮬레이션 업데이트
	_scene->simulate(tick);
	_scene->fetchResults(true);
}

bool core::PhysicsScene::CreatePhysicsActor(const core::Entity& entity)
{
	using namespace physx;

	// 필수 컴포넌트 검사
	if (!entity.HasAllOf<Transform, ColliderCommon>())
		return false;

	// 중복 검사
	if (_entityToPxActorMap.contains(entity))
		return false;

	const auto& collider = entity.Get<ColliderCommon>();
	const auto& transform = entity.Get<Transform>();

	// 물리 Material 생성
	PxMaterial* material = _physics->createMaterial(
		collider.material.staticFriction,
		collider.material.dynamicFriction,
		collider.material.bounciness
	);

	PxShape* shape = nullptr;
	PxActor* actor = nullptr;

	// Collider 할당
	if (entity.HasAllOf<BoxCollider>())
	{
		const auto& box = entity.Get<BoxCollider>();

		if (collider.sharedMaterial.empty())
			shape = _physics->createShape(PxBoxGeometry(box.size.x / 2, box.size.y / 2, box.size.z / 2), *material, true);
		else
			shape = shape = _physics->createShape(PxBoxGeometry(box.size.x / 2, box.size.y / 2, box.size.z / 2), *material);
	}
	else if (entity.HasAllOf<SphereCollider>())
	{
		const auto& sphere = entity.Get<SphereCollider>();

		if (collider.sharedMaterial.empty())
			shape = _physics->createShape(PxSphereGeometry(sphere.radius), *material, true);
		else
			shape = _physics->createShape(PxSphereGeometry(sphere.radius), *material);
	}
	else if (entity.HasAllOf<CapsuleCollider>())
	{
		const auto& capsule = entity.Get<CapsuleCollider>();

		if (collider.sharedMaterial.empty())
			shape = _physics->createShape(PxCapsuleGeometry(capsule.radius, capsule.height), *material, true);
		else
			shape = _physics->createShape(PxCapsuleGeometry(capsule.radius, capsule.height), *material);
	}
	else if (entity.HasAllOf<MeshCollider>()) // todo
	{
		const auto& mesh = entity.Get<MeshCollider>();
	}
	else
	{
		material->release();
		return false;
	}

	if (!shape)
	{
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

	// 매핑 테이블에 액터 추가 및 물리 씬에 추가
	_entityToPxActorMap[entity] = actor;
	_scene->addActor(*actor);

	shape->release();
	material->release();

	return true;
}

bool core::PhysicsScene::DestroyPhysicsActor(const Entity& entity)
{
	using namespace physx;

	// 엔티티와 매핑된 액터 찾기

	if (const auto it = _entityToPxActorMap.find(entity); it != _entityToPxActorMap.end())
	{
		// 액터를 물리 씬에서 제거
		PxActor* actor = it->second;
		_scene->removeActor(*actor);

		// 액터 메모리 해제
		if (actor->is<PxRigidDynamic>())
		{
			actor->is<PxRigidDynamic>()->release();
		}
		else if (actor->is<PxRigidStatic>())
		{
			actor->is<PxRigidStatic>()->release();
		}

		// 매핑에서 엔티티 제거
		_entityToPxActorMap.erase(it);

		return true;
	}

	return false;
}

void core::PhysicsScene::Clear()
{
	using namespace physx;

	for (const auto& actor : _entityToPxActorMap | std::views::values)
	{
		if (actor != nullptr)
		{
			// 액터를 물리 씬에서 제거
			_scene->removeActor(*actor, true);

			// 액터 메모리 해제
			if (actor->is<PxRigidDynamic>())
			{
				actor->is<PxRigidDynamic>()->release();
			}
			else if (actor->is<PxRigidStatic>())
			{
				actor->is<PxRigidStatic>()->release();
			}
		}
	}

	// 매핑 정보 클리어
	_entityToPxActorMap.clear();
}
