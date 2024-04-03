#include "pch.h"

#include "Components.h"
#include "Scene.h"
#include "Systems.h"

#include "Components.h"
#include "ComponentTemplates.h"

int main()
{
	using namespace engine;

	// register meta data
	{
		entt::meta<Transform>()
			.type(entt::type_hash<Transform>::value())
			.data<&Transform::x>("x"_hs)
			.data<&Transform::y>("y"_hs)
			.data<&Transform::z>("z"_hs)
			.func<&engine::SaveSnapshot<Transform>>("SaveSnapshot"_hs)
			.func<&engine::LoadSnapshot<Transform>>("LoadSnapshot"_hs)
			.func<&engine::SavePrefabSnapshot<Transform>>("SavePrefabSnapshot"_hs)
			.func<&engine::LoadPrefabSnapshot<Transform>>("LoadPrefabSnapshot"_hs);


		entt::meta<Relationship>()
			.type(entt::type_hash<Relationship>::value())
			.data<&Relationship::parent>("parent"_hs)
			.func<&engine::SaveSnapshot<Relationship>>("SaveSnapshot"_hs)
			.func<&engine::LoadSnapshot<Relationship>>("LoadSnapshot"_hs)
			.func<&engine::SavePrefabSnapshot<Relationship>>("SavePrefabSnapshot"_hs)
			.func<&engine::LoadPrefabSnapshot<Relationship>>("LoadPrefabSnapshot"_hs);
	}

	engine::Scene scene1;

	auto e1 = scene1.AddEntity();
	auto e2 = scene1.AddEntity();
	auto e3 = scene1.AddEntity();
	auto e4 = scene1.AddEntity();

	e1.SetParent(e2);
	e2.SetParent(e3);

	e1.Emplace<engine::Transform>();
	e2.Emplace<engine::Transform>();
	e3.Emplace<engine::Transform>();

	scene1.RegisterSystem<engine::TransformSystem>();
	scene1.RemoveSystem<engine::TransformSystem>();

	auto b1 = e4.IsAncestorOf(e1);
	auto b2 = e1.IsDescendantOf(e4);

	scene1.Serialize("../test.json");
	//scene.Deserialize("../test.json");

	scene1.SavePrefab("../prefab.json", e1);
	scene1.LoadPrefab("../prefab.json");

	while (true)
		scene1.Run();

	return 0;
}
