#include "pch.h"
#include "Scene.h"

#include "Components.h"
#include <cereal/archives/json.hpp>

#include "JsonOutputArchive.h"
#include "JsonInputArchive.h"


struct Tester
{

};

engine::Entity engine::Scene::AddEntity()
{
	return { _registry.create(), this->_registry };
}

bool engine::Scene::Serialize()
{
	// registry Serialize/Deserialize
	{
		std::stringstream ss;
		nlohmann::json json;

		// Serialize
		{
			//cereal::JSONOutputArchive output{ ss };
			JsonOutputArchive output;

			auto view = _registry.view<Transform>();

			for (auto&& [entity, transform] : view.each())
			{
				if (static_cast<uint32_t>(entity) == 0)
				{
					transform.x = 0;
					transform.y = 1;
					transform.z = 2;
				}
				else if (static_cast<uint32_t>(entity) == 1)
				{
					transform.x = 1;
					transform.y = 2;
					transform.z = 3;
				}
				else if (static_cast<uint32_t>(entity) == 2)
				{
					transform.x = 2;
					transform.y = 3;
					transform.z = 4;
				}
			}

			entt::snapshot snapshot(_registry);
			snapshot.get<engine::Relationship>(output, view.begin(), view.end());
			snapshot.get<engine::Transform>(output, view.begin(), view.end());

			output.Close();
			//auto str = output.AsString();
			json = output.AsString();
		}


		auto str = ss.str();

		// Deserialize
		{
			//cereal::JSONInputArchive input{ ss };
			JsonInputArchive input{ json };

			entt::registry dest;
			entt::snapshot_loader loader(dest);

			loader.get<Transform>(input);
			loader.get<Relationship>(input);

			auto v1 = dest.view<Transform>();
			auto size1 = v1.size();

			for(auto&& [entity, transform] : v1.each())
			{
				int i = 0;
			}

			auto v2 = dest.view<Relationship>();
			auto size2 = v2.size();

			for (auto&& [entity, relationship] : v2.each())
			{
				int i = 0;
			}
		}
	}

	auto factory = entt::meta<Relationship>();
	auto factory2 = entt::meta<Transform>();
	auto factory3 = entt::meta<Tester>();



	auto m1 = entt::meta<Transform>()
		.type(entt::type_hash<Transform>::value())
		.data<&Transform::x>("x"_hs)
		.data<&Transform::y>("y"_hs)
		.data<&Transform::z>("z"_hs)
		.func<&Transform::Deserialize>("Deserialize"_hs);

	for (auto&& [id, type] : entt::resolve())
	{
		auto n1 = type.info().name();
		auto any = type.construct();

		auto d1 = type.data("x"_hs);
		auto d2 = type.data("y"_hs);
		auto d3 = type.data("z"_hs);

		if (d1)
		{
			d1.set(any, 19);
			d2.set(any, 20);
			d3.set(any, 21);

			auto func = type.func("Deserialize"_hs);
			auto meta = func.invoke(any, &_registry);
		}
	}

	return false;
}

bool engine::Scene::Deserialize(std::filesystem::path& path)
{
	// 시스템 메타데이터 역직렬화

	// 시스템 추가

	return false;
}

void engine::Scene::Run()
{
}

std::vector<engine::Scene::SystemCallback>& engine::Scene::GetSystemContainer(SystemType type)
{
	switch (type)
	{
	case SystemType::Update:
		return _updateSystems;
	case SystemType::FixedUpdate:
		return _fixedUpdateSystems;
	case SystemType::Render:
		return _renderSystems;
	default:
		throw std::runtime_error("Invalid system type");
	}
}
