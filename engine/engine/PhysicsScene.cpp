#include "pch.h"
#include "PhysicsScene.h"

#include "Entity.h"
#include "Components.h"

static physx::PxMaterial* gMaterial = nullptr;
static physx::PxReal stackZ = 10.0f;

static physx::PxDefaultAllocator allocator;
static physx::PxDefaultErrorCallback errorCallback;

core::PhysicsScene::PhysicsScene()
{
	using namespace physx;

	// PxFoundation 생성
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

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
	gMaterial = _physics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*_physics, PxPlane(0, 1, 0, 0), *gMaterial);
	_scene->addActor(*groundPlane);

	for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
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

void core::PhysicsScene::createStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
{
	using namespace physx;

	PxShape* shape = _physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = _physics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			_scene->addActor(*body);
		}
	}
	shape->release();
}

void core::PhysicsScene::AddRigidbody(Entity& entity, const Rigidbody& rigidbody)
{
	// Entity에 연결된 PxActor를 가져오거나 생성합니다.
	physx::PxRigidDynamic* dynamicActor = _physics->createRigidDynamic(physx::PxTransform(physx::PxVec3(0, 0, 0)));

	// Rigidbody 속성을 기반으로 물리 속성 설정
	dynamicActor->setMass(rigidbody.mass);
	dynamicActor->setLinearDamping(rigidbody.drag);
	dynamicActor->setAngularDamping(rigidbody.angularDrag);
	dynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rigidbody.useGravity);
	dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody.isKinematic);

	// Entity와 PxRigidDynamic 연결 (가정)
	//entity.SetPhysxActor(dynamicActor);

	// Scene에 액터 추가
	_scene->addActor(*dynamicActor);
}

void core::PhysicsScene::AddCollider(Entity& entity, const Collider& collider)
{
	// Entity에 연결된 PxActor를 가져옵니다. 여기서는 PxRigidDynamic으로 가정합니다.
	//physx::PxRigidActor* actor = static_cast<physx::PxRigidActor*>(entity.GetPhysxActor());
	physx::PxRigidActor* actor = nullptr;
	if (!actor) return; // 오류 처리

	// Collider의 물리 재질 생성
	physx::PxMaterial* material = _physics->createMaterial(collider.material.dynamicFriction, collider.material.staticFriction, collider.material.bounciness);

	// 예시: 박스 콜라이더 생성. 실제로는 Collider 구조체에 따라 다른 형태의 콜라이더를 생성해야 할 수 있습니다.
	physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *material);

	// 콜라이더가 트리거인지 설정
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider.isTrigger);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider.isTrigger);

	// 액터에 콜라이더(Shape) 부착
	actor->attachShape(*shape);

	// 사용이 끝난 후 PxMaterial과 PxShape를 해제
	shape->release();
	material->release();
}
