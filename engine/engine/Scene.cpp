#include "pch.h"
#include "Scene.h"

engine::Entity engine::Scene::AddEntity()
{
	return { _registry.create(), this->_registry };
}

bool engine::Scene::Serialize(std::filesystem::path& path)
{
	// 시스템 메타데이터 직렬화

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
