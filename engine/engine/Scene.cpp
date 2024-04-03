#include "pch.h"
#include "Scene.h"

#include "Components.h"
#include "ComponentTemplates.h"

#include <fstream>

engine::Entity engine::Scene::AddEntity()
{
	return { _registry.create(), this->_registry };
}

bool engine::Scene::Serialize(const std::string& path)
{
	std::stringstream ss;

	{
		entt::snapshot snapshot(_registry);
		cereal::JSONOutputArchive archive(ss);

		snapshot.get<entt::entity>(archive);

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto save = type.func("SetSnapshot"_hs))
				save.invoke({}, &snapshot, &archive);
		}
	}

	std::ofstream outFile(path, std::ios::out | std::ios::trunc);

	if (outFile)
	{
		outFile << ss.str();
		outFile.close();
	}
	else
	{
		return false;
	}

	return true;
}

bool engine::Scene::Deserialize(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		return false;
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	{
		cereal::JSONInputArchive archive(ss);
		entt::snapshot_loader loader(_registry);
		loader.get<entt::entity>(archive);

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto load = type.func("LoadSnapshot"_hs))
				load.invoke({}, &loader, &archive);
		}

		loader.orphans();
	}

	return true;
}

void engine::Scene::Run()
{
}

void engine::Scene::UpdateSystemMapIndex(SystemType type, size_t oldIndex, size_t newIndex)
{
	// 시스템 맵에서 스왑된 시스템을 찾아 인덱스를 업데이트
	for (auto& pair : _systemMap | std::views::values) 
	{
		if (pair.first == type && pair.second == oldIndex) 
		{
			pair.second = newIndex;
			break;
		}
	}
}
