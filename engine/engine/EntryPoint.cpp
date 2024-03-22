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

	e1.Emplace<engine::Parent>().handle = e2;
	e2.Emplace<engine::Parent>().handle = e3;
	e3.Emplace<engine::Parent>().handle = e4;

	e2.Remove<engine::Parent>();

	scene.RegisterSystem<engine::TransformSystem>();
	scene.RemoveSystem<engine::TransformSystem>();

	auto b1 = e4.IsAncestorOf(e1);
	auto b2 = e1.IsDescendantOf(e4);

	while (true)
		scene.Run();

	return 0;
}