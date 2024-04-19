#include "pch.h"

#include "Scene.h"
#include "MetaFuncs.h"

/*
 * 생각 해야할 것
 * - 모든 실행의 중심은 시스템이라는 것을 인지하고 만들어야 함
 * - 엔티티가 직접 통신하는 것이 아닌, 시스템 간의 통신으로 이루어짐
 *
 * 1. 충돌 처리
 *	1.1. 모든 충돌 처리 시스템이 이벤트를 받고 선택적으로 작동한다, 쉽지만 성능상 나쁨
 *	1.2. 중앙 충돌 관리 시스템에서 물리 씬으로부터 이벤트를 받고 이를 재분배한다, 어떻게?
 *		- 해결 방안: Tag 컴포넌트를 이용하여 특정 태그를 가진 엔티티의 충돌만을 관리하는 시스템을 만들 수 있게 함
 *
 * 2. 씬 전환
 *	2.1. 최상위 dispatcher 를 두고 씬 간에 통신한다, 시스템도 이걸 알아야하나?
 *	2.2. 
 *
 * 3. 씬 로드
 *	3.1. 씬을 메모리에 미리 올려놓는다 (많은 메모리 사용, 빠른 전환), 쉽지만 메모리상 (매우)나쁨
 *	3.2. 씬을 전환할 때마다 파일을 읽는다 (메모리 최소화, 느린 전환), 쉽지만 느림
 *	3.3. 전환 가능한 씬만 메모리에 올린다 (메모리 절약, 빠른 전환), 좋지만 어떻게?
 *
 * 4. FSM
 */

int main()
{
	using namespace core;

	RegisterCoreMetaData();

	core::Scene scene1;
	core::Scene scene2;

	auto e1 = scene1.CreateEntity();
	auto e2 = scene1.CreateEntity();
	auto e3 = scene1.CreateEntity();
	auto e4 = scene1.CreateEntity();

	e1.SetParent(e2);
	e2.SetParent(e3);

	e1.Emplace<core::Transform>();
	e2.Emplace<core::Transform>();
	e3.Emplace<core::Transform>();


	for (auto i = 0; i < 3; i++)
	{
		auto entity = scene1.CreateEntity();

		auto& tag = entity.Emplace<Tag>();
		tag.id = tag::Untagged::id;

		auto& layer = entity.Emplace<Layer>();
		layer.id = layer::Default::id;

		auto& transform = entity.Emplace<Transform>();
		transform.position = Vector3{ (2.f * i), (2.f * i), (2.f * i) };

		entity.Emplace<ColliderCommon>();
		auto& boxCollider = entity.Emplace<BoxCollider>();
		boxCollider.size = Vector3{ 4.f, 4.f, 4.f };

		auto& rigidbody = entity.Emplace<core::Rigidbody>();
		rigidbody.mass = 3.0f;
		rigidbody.useGravity = true;
	}

#pragma region meta_test
	//{
	//	int number = 0;

	//	for (auto&& [id, meta] : entt::resolve(global::componentMetaCtx))
	//	{
	//		std::cout << ++number << ": ";
	//		std::cout << meta.info().name() << std::endl;
	//	}

	//	int input = 0;
	//	std::cin >> input;

	//	if (input > 0)
	//	{
	//		int counter = 0;
	//		for (auto&& [id, meta] : entt::resolve(global::componentMetaCtx))
	//		{
	//			++counter;
	//			if(counter == input)
	//			{
	//				std::cout << "constructed component: ";
	//				std::cout << meta.info().name() << std::endl;
	//				auto component = meta.construct();
	//				meta.func("Assign"_hs).invoke({}, scene1.GetRegistry(), &e1.GetHandle(), &component);
	//				counter = 0;
	//				break;
	//			}
	//		}
	//	}
	//}
#pragma endregion

	scene1.RegisterSystem<core::TransformSystem>();
	scene1.RegisterSystem<core::AnimationSystem>();
	scene1.RegisterSystem<core::PhysicsSystem>();
	scene1.RegisterSystem<core::CollisionTesterSystem>();

	auto b1 = e4.IsAncestorOf(e1);
	auto b2 = e1.IsDescendantOf(e4);

	scene1.SaveScene("../sceneTest");
	scene2.LoadScene("../sceneTest");

	//scene1.SavePrefab("../prefabTest", e1);
	//scene1.LoadPrefab("../prefabTest");

	scene1.Start();

	while (true)
	{
		scene1.Run();
	}

	scene1.Finish();

	return 0;
}