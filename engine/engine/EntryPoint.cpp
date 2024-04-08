#include "pch.h"

#include "PhysicsScene.h"
#include "Scene.h"
#include "Utils.h"

int main()
{
	using namespace core;

	RegisterMetaData();

	core::Scene scene1;

	auto e1 = scene1.AddEntity();
	auto e2 = scene1.AddEntity();
	auto e3 = scene1.AddEntity();
	auto e4 = scene1.AddEntity();

	e1.SetParent(e2);
	e2.SetParent(e3);

	e1.Emplace<core::Transform>();
	e2.Emplace<core::Transform>();
	e3.Emplace<core::Transform>();
	e3.Emplace<core::ColliderCommon>();
	e3.Emplace<core::BoxCollider>();
	e3.Emplace<core::Rigidbody>();


	scene1.RegisterSystem<core::TransformSystem>();
	scene1.RegisterSystem<core::AnimationSystem>();
	scene1.RegisterSystem<core::PhysicsSystem>();
	scene1.RemoveSystem<core::AnimationSystem>();

	auto b1 = e4.IsAncestorOf(e1);
	auto b2 = e1.IsDescendantOf(e4);

	scene1.SaveScene("../sceneTest");
	//scene1.LoadScene("../sceneTest");

	/*scene1.SavePrefab("../prefabTest", e1);
	scene1.LoadPrefab("../prefabTest");*/

	while (true)
	{
		scene1.Run();
	}

	return 0;
}