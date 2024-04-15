#include "pch.h"

#include "Scene.h"
#include "Utils.h"

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

	for (auto i = 0; i < 10; i++)
	{
		auto entity = scene1.CreateEntity();

		auto& transform = entity.Emplace<Transform>();
		transform.position = Vector3{ (2.f * i), (2.f * i), (2.f * i) };

		entity.Emplace<ColliderCommon>();
		auto& boxCollider = entity.Emplace<BoxCollider>();
		boxCollider.size = Vector3{ 4.f, 4.f, 4.f };

		auto& rigidbody = entity.Emplace<core::Rigidbody>();
	}

#pragma region meta_test
	//{
	//	int number = 0;

	//	for (auto&& [id, meta] : entt::resolve(componentMetaCtx))
	//	{
	//		std::cout << ++number << ": ";
	//		std::cout << meta.info().name() << std::endl;
	//	}

	//	int input = 0;
	//	std::cin >> input;

	//	if (input > 0)
	//	{
	//		int counter = 0;
	//		for (auto&& [id, meta] : entt::resolve(componentMetaCtx))
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
	scene1.RegisterSystem<core::EventTestSystem>();

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