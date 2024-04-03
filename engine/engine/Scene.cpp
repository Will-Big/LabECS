#include "pch.h"
#include "Scene.h"

#include "Components.h"
#include "ComponentTemplates.h"

#include <fstream>

engine::Entity engine::Scene::AddEntity()
{
	return { _registry.create(), this->_registry };
}

bool engine::Scene::Serialize(std::string_view path)
{
	std::stringstream ss;

	// 씬 스냅샷 생성
	{
		entt::snapshot snapshot(_registry);
		cereal::JSONOutputArchive archive(ss);

		snapshot.get<entt::entity>(archive);

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto save = type.func("SaveSnapshot"_hs))
				save.invoke({}, &snapshot, &archive);
		}
	}

	std::ofstream outFile(path.data(), std::ios::out | std::ios::trunc);

	// 씬 스냅샷 파일 저장
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

bool engine::Scene::Deserialize(std::string_view path)
{
	std::ifstream file(path.data());

	if (!file.is_open())
	{
		return false;
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	// 씬 스냅샷 로드
	{
		cereal::JSONInputArchive archive(ss);
		entt::snapshot_loader loader(_registry);
		loader.get<entt::entity>(archive);

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto load = type.func("LoadSnapshot"_hs))
				load.invoke({}, &loader, &archive);
		}

		// 불필요 Entity 제거
		loader.orphans();
	}

	return true;
}

bool engine::Scene::SavePrefab(std::string_view path, engine::Entity& entity)
{
	// 계층구조 저장용 벡터
	std::vector<entt::entity> descendents;
	descendents.push_back(entity);

	// 하위 Entity 저장
	auto entities = _registry.view<entt::entity>();
	for (auto& element : entities)
	{
		auto tempEntity = Entity{ element, _registry };

		if (entity.IsAncestorOf(tempEntity))
			descendents.push_back(tempEntity);
	}

	// 프리팹 스냅샷 생성
	std::stringstream ss;
	{
		// 상위 계층구조 존재시 임시 해제
		bool hasParent = entity.HasAllOf<Relationship>();
		Relationship relationship{};

		if (hasParent)
		{
			relationship = entity.Get<Relationship>();
			entity.Remove<Relationship>();
		}

		{
			entt::snapshot snapshot(_registry);
			cereal::JSONOutputArchive archive(ss);

			snapshot.get<entt::entity>(archive);

			for (auto&& [id, type] : entt::resolve())
			{
				if (auto save = type.func("SavePrefabSnapshot"_hs))
					save.invoke({}, &snapshot, &archive, descendents.begin(), descendents.end());
			}
		}

		// 계층구조 재연결
		if(hasParent)
		{
			entity.Emplace<Relationship>(relationship);
		}
	}
	
	std::ofstream outFile(path.data(), std::ios::out | std::ios::trunc);

	// 프리팹 파일 저장
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

bool engine::Scene::LoadPrefab(std::string_view path)
{
	std::ifstream file(path.data());

	if (!file.is_open())
	{
		return false;
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	// 프리팹 스냅샷 로드
	{
		cereal::JSONInputArchive archive(ss);
		entt::continuous_loader loader(_registry);
		loader.get<entt::entity>(archive);

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto load = type.func("LoadPrefabSnapshot"_hs))
				load.invoke({}, &loader, &archive);
		}

		// 불필요 Entity 제거
		loader.orphans();
	}

	auto v1 = _registry.view<entt::entity>();

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
