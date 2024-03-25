#include "pch.h"

#include "Components.h"
#include "Scene.h"
#include "Systems.h"

#include "entt/entt.hpp"
#include "cereal/archives/json.hpp"


struct position
{
	int x;
	int y;
};

template<typename Archive>
void serialize(Archive& archive, position& position) {
	archive(position.x, position.y);
}

struct relationship
{
	entt::entity parent;
};

template<typename Archive>
void serialize(Archive& archive, relationship& relationship) {
	archive(relationship.parent);
}

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


	/*entt::registry registry;

	auto e1 = registry.create();
	auto e2 = registry.create();
	auto e3 = registry.create();

	auto& p1 = registry.emplace<position>(e1);
	p1.x = 1;
	p1.y = 1;
	auto& r1 = registry.emplace<relationship>(e1);
	r1.parent = e2;

	auto& p2 = registry.emplace<position>(e2);
	p2.x = 2;
	p2.y = 2;
	auto& r2 = registry.emplace<relationship>(e2);
	r2.parent = e3;

	auto& p3 = registry.emplace<position>(e3);
	p3.x = 3;
	p3.y = 3;


	std::stringstream ss;
	cereal::JSONOutputArchive output{ ss };*/


	return 0;
}