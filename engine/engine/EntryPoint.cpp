#include "pch.h"

#include "Components.h"
#include "Scene.h"
#include "Systems.h"

int main()
{
	engine::Scene scene;

	auto e1 = scene.AddEntity();
	auto e2 = scene.AddEntity();
	auto e3 = scene.AddEntity();
	auto e4 = scene.AddEntity();

	e1.SetParent(e2);
	e2.SetParent(e3);
	e3.SetParent(e4);

	e1.Emplace<engine::Transform>();
	e2.Emplace<engine::Transform>();
	e3.Emplace<engine::Transform>();

	scene.RegisterSystem<engine::TransformSystem>();
	scene.RemoveSystem<engine::TransformSystem>();

	auto b1 = e4.IsAncestorOf(e1);
	auto b2 = e1.IsDescendantOf(e4);

	scene.Serialize();

	while (true)
		scene.Run();

	return 0;
}