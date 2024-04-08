#include "pch.h"
#include "Scene.h"

#include "Systems.h"
#include "Components.h"
#include "PhysicsScene.h"

#include <fstream>
#include <cereal/types/vector.hpp>

core::Scene::Scene()
{
	_physicsScene = std::make_unique<PhysicsScene>();
}

core::Entity core::Scene::AddEntity()
{
	return { _registry.create(), this->_registry };
}

bool core::Scene::SaveScene(const std::string& path)
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

		// 시스템 이름 추가
		std::vector<std::string> systemNames;
		for (const auto& name : _systemMap | std::views::keys)
		{
			auto it = std::ranges::find(systemNames, name);

			if (it == systemNames.end())
				systemNames.push_back(name);
		}
		archive(cereal::make_nvp("systems", systemNames));
	}

	auto str = ss.str();

	std::ofstream outFile(path + ".scene", std::ios::out | std::ios::trunc);

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

bool core::Scene::LoadScene(const std::string& path)
{
	std::ifstream file(path + ".scene");

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
			if (auto loadStorage = type.func("LoadSnapshot"_hs))
				loadStorage.invoke({}, &loader, &archive);
		}

		// 로드할 시스템 목록
		std::vector<std::string> systemNames;
		archive(cereal::make_nvp("systems", systemNames));

		for (auto&& [id, type] : entt::resolve())
		{
			if (auto loadSystem = type.func("LoadSystem"_hs))
				loadSystem.invoke({}, this, &systemNames);
		}

		// 불필요 엔터티 제거
		loader.orphans();
	}

	return true;
}

bool core::Scene::SavePrefab(const std::string& path, core::Entity& entity)
{
	// 계층구조 저장용 벡터
	std::vector<entt::entity> descendents;
	descendents.push_back(entity);

	// 하위 엔터티 저장
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
		if (hasParent)
		{
			entity.Emplace<Relationship>(relationship);
		}
	}

	std::ofstream outFile(path + ".prefab", std::ios::out | std::ios::trunc);

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

bool core::Scene::LoadPrefab(const std::string& path)
{
	std::ifstream file(path + ".prefab");

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

		// 불필요 엔터티 제거
		loader.orphans();
	}

	return true;
}

void core::Scene::Run()
{
	for (auto& update : _updates)
	{
		(*update)(_registry, 0.016f);
	}

	for (auto& fixed : _fixeds)
	{
		(*fixed)(_registry, 0.016f);
	}

	
}

void core::Scene::UpdateSystemMapIndex(SystemType type, size_t oldIndex, size_t newIndex)
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
