#include "pch.h"
#include "PhysicsScene.h"

#include "Scene.h"
#include "Entity.h"
#include "MetaCtxs.h"
#include "CoreComponents.h"
#include "CollisionCallback.h"
#include "CoreTagsAndLayers.h"
#include "CoreComponentInlines.h"
#include "CorePhysicsComponents.h"

core::PhysicsScene::PhysicsScene(Scene& scene)
	: _scene(&scene)
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
	sceneDesc.filterShader = customFilterShader;
	_pxScene = _physics->createScene(sceneDesc);
	_pxScene->setSimulationEventCallback(new CollisionCallback(*_scene));

	PxPairFlags desiredFlags = PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

	if (PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient())
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	//auto gMaterial = _physics->createMaterial(0.5f, 0.5f, 0.6f);
	//PxRigidStatic* groundPlane = PxCreatePlane(*_physics, PxPlane(0, 1, 0, 0), *gMaterial);
	//_pxScene->addActor(*groundPlane);
}

core::PhysicsScene::~PhysicsScene()
{
	using namespace physx;

	Clear();

	delete _pxScene->getSimulationEventCallback();
	_pxScene->release();
	_pvd->release();
	_physics->release();
	_foundation->release();
}

void core::PhysicsScene::Update(float tick)
{
	using namespace physx;

	// 시뮬레이션 업데이트
	_pxScene->simulate(tick);
	_pxScene->fetchResults(true);
}

void core::PhysicsScene::CreatePhysicsActor(const core::Entity& entity)
{
	using namespace physx;

	// 필수 컴포넌트 검사
	if (!entity.HasAllOf<Transform, ColliderCommon>())
		return;

	// 중복 검사
	if (_entityToPxActorMap.contains(entity))
		return;

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
		throw std::runtime_error("Entity must contain at least one collider");
	}

	if (!shape)
	{
		material->release();
		throw std::runtime_error("Failed to assign Shape");
	}

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !collider.isTrigger);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, collider.isTrigger);

	if (entity.HasAllOf<Rigidbody>())
	{
		const auto& rigid = entity.Get<Rigidbody>();

		// 동적 액터 생성
		PxRigidDynamic* dynamicActor = _physics->createRigidDynamic(
			PxTransform(
				convert<PxVec3>(transform.position),
				convert<PxQuat>(transform.rotation)
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
				convert<PxVec3>(transform.position),
				convert<PxQuat>(transform.rotation)
			)
		);

		staticActor->attachShape(*shape);

		actor = staticActor;
	}

	shape->release();
	material->release();

	if (!actor)
		throw std::runtime_error("Failed to assign Actor");

	// 레이어 컴포넌트를 사용하여 레이어 ID를 확인
	if (entity.HasAllOf<Layer>())
	{
		const auto& layerComponent = entity.Get<Layer>();
		uint32_t layerId = layerComponent.id; // 레이어 ID 가져오기

		if (_layerIdToIndexMap.contains(layerId))
		{
			uint32_t layerIndex = _layerIdToIndexMap[layerId];		// 레이어 인덱스 조회
			uint32_t collisionGroup = 1u << layerIndex;				// 충돌 그룹: 레이어 인덱스에 해당하는 비트 위치
			uint32_t collisionMask = _collisionMatrix[layerIndex];	// 충돌 마스크: 해당 레이어에 대한 충돌 가능한 레이어 마스크

			physx::PxFilterData filterData;
			filterData.word0 = collisionGroup;	// 충돌 그룹 설정
			filterData.word1 = collisionMask;	// 충돌 마스크 설정

			shape->setSimulationFilterData(filterData); // 필터 데이터 설정
		}
		else
		{
			actor->release();
			throw std::runtime_error("Layer ID does not exist in the layer map.");
		}
	}
	else
	{
		// Layer 컴포넌트가 없는 경우, 기본 필터 데이터 설정
		physx::PxFilterData filterData;
		filterData.word0 = 1u;			// 기본 충돌 그룹 (layer::Default)
		filterData.word1 = 0xFFFFFFFF;	// 모든 충돌을 허용
		shape->setSimulationFilterData(filterData);
	}

	// userData 할당
	entt::entity* entityPtr = new entt::entity(entity.GetHandle());
	actor->userData = static_cast<void*>(entityPtr);

	// 매핑 테이블에 액터 추가 및 물리 씬에 추가
	_entityToPxActorMap[entity] = actor;
	_pxScene->addActor(*actor);
}

void core::PhysicsScene::DestroyPhysicsActor(const Entity& entity)
{
	using namespace physx;

	// 엔티티와 매핑된 액터 찾기
	if (const auto it = _entityToPxActorMap.find(entity); it != _entityToPxActorMap.end())
	{
		// 액터를 물리 씬에서 제거
		PxActor* actor = it->second;
		_pxScene->removeActor(*actor);
		delete static_cast<entt::entity*>(actor->userData);

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
	}

	// 존재하지 않는 액터의 경우 에러 처리
	throw std::runtime_error("Entity does not exist in Physics Scene");
}

void core::PhysicsScene::AddLayer(entt::id_type layerId)
{
	if (!_layerIdToIndexMap.contains(layerId))
	{
		uint32_t layerIndex = 0;

		// 메타데이터에서 layerId에 해당하는 레이어 정보 추출
		for (auto&& [id, type] : entt::resolve(global::layerMetaCtx))
		{
			if (id == layerId) {

				if (auto prop = type.prop("index"_hs))
				{
					layerIndex = prop.value().cast<uint32_t>();
					if (layerIndex < _collisionMatrix.size())
					{
						_layerIdToIndexMap[layerId] = layerIndex;
						_collisionMatrix[layerIndex] = 0xFFFFFFFF; // 모든 충돌을 허용하도록 초기화
						return;
					}
				}
				break;
			}
		}

		// 적절한 인덱스가 제공되지 않았거나 범위를 벗어난 경우 예외 처리
		throw std::runtime_error("Invalid layer index or layer index out of range");
	}
}

void core::PhysicsScene::SetLayerCollision(entt::id_type layerId1, entt::id_type layerId2, bool canCollide)
{
	if (!_layerIdToIndexMap.contains(layerId1) || !_layerIdToIndexMap.contains(layerId2)) 
	{
		throw std::runtime_error("One of the layer IDs does not exist.");
	}

	uint32_t index1 = _layerIdToIndexMap[layerId1];
	uint32_t index2 = _layerIdToIndexMap[layerId2];

	if (canCollide)
	{
		_collisionMatrix[index1] |= (1 << index2);
		_collisionMatrix[index2] |= (1 << index1);
	}
	else
	{
		_collisionMatrix[index1] &= ~(1 << index2);
		_collisionMatrix[index2] &= ~(1 << index1);
	}
}

void core::PhysicsScene::sceneFetch(const Entity& entity)
{
	using namespace physx;

	auto& transform = entity.Get<Transform>();

	PxTransform pxTransform;

	auto it = _entityToPxActorMap.find(entity);

	if (it == _entityToPxActorMap.end())
		return;

	if (auto dynamicActor = it->second->is<PxRigidDynamic>())
	{
		pxTransform = dynamicActor->getGlobalPose();
		const PxVec3 pxVec3 = dynamicActor->getLinearVelocity();

		auto& rigidbody = entity.Get<Rigidbody>();
		rigidbody.velocity = convert<Vector3>(pxVec3);
	}
	else if (auto staticActor = it->second->is<PxRigidStatic>())
	{
		pxTransform = staticActor->getGlobalPose();
	}

	// PhysX의 오른손 좌표계에서 왼손 좌표계로 변환
	pxTransform = rightToLeft(pxTransform);

	// Transform 컴포넌트 업데이트
	transform.position = convert<Vector3>(pxTransform.p);
	transform.rotation = convert<Quaternion>(pxTransform.q);
}

physx::PxFilterFlags core::PhysicsScene::customFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	using namespace physx;

	// 트리거(충돌 감지 전용) 확인
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// 가장 낮은 비트의 위치 반환
	uint32_t layerIndex0 = std::countr_zero(filterData0.word0);
	uint32_t layerIndex1 = std::countr_zero(filterData1.word0);

	// 비트 연산으로 충돌 여부 확인
	if (!(_collisionMatrix[layerIndex0] & (1 << layerIndex1))) 
	{
		return PxFilterFlag::eSUPPRESS;  // 충돌을 발생시키지 않을 경우
	}

	// 콜백 설정
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT;
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;

	return PxFilterFlag::eDEFAULT;
}

physx::PxTransform core::PhysicsScene::rightToLeft(const physx::PxTransform& rightHandTransform)
{
	using namespace physx;

	// 위치의 z 좌표를 반전
	PxVec3 position = rightHandTransform.p;
	position.z = -position.z;

	// 회전의 z축 반전을 위해 적절한 회전 조정
	PxQuat rotation = rightHandTransform.q;
	PxQuat zFlip(0, 0, 1, 0); // 180도 z축 회전
	rotation = rotation * zFlip;

	return PxTransform(position, rotation);
}

physx::PxTransform core::PhysicsScene::leftToRight(const physx::PxTransform& leftHandTransform)
{
	using namespace physx;

	// 위치의 z 좌표를 반전
	PxVec3 position = leftHandTransform.p;
	position.z = -position.z;

	// 회전의 z축 반전을 위해 적절한 회전 조정
	PxQuat rotation = leftHandTransform.q;
	PxQuat zFlip(0, 0, 1, 0); // 180도 z축 회전
	rotation = rotation * zFlip;

	return PxTransform(position, rotation);
}

void core::PhysicsScene::Clear()
{
	using namespace physx;

	for (const auto& actor : _entityToPxActorMap | std::views::values)
	{
		if (actor != nullptr)
		{
			// 액터를 물리 씬에서 제거
			_pxScene->removeActor(*actor, true);
			delete static_cast<entt::entity*>(actor->userData);

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
