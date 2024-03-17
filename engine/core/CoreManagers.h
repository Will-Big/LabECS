#pragma once

namespace core
{
	class EntityManager;
	class ComponentManager;

	// core 라이브러리의 매니저 전달용 구조체
	struct CoreManagers
	{
		std::shared_ptr<EntityManager> entity;
		std::shared_ptr<ComponentManager> component;
	};
}

