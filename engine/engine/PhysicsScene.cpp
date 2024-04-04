#include "pch.h"
#include "PhysicsScene.h"

static physx::PxMaterial* gMaterial = NULL;
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
