#include "pch.h"

#include "Components.h"
#include "Scene.h"
#include "Systems.h"

int main()
{
	engine::Scene scene;

	auto e1 = scene.AddEntity();

	scene.RegisterSystem<engine::TransformSystem>();
	scene.RemoveSystem<engine::TransformSystem>();

	while (true)
		scene.Run();

	return 0;
}