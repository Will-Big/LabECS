#pragma once
#include "Scene.h"
#include "SystemTraits.h"

namespace engine
{
	class TransformSystem
	{
	public:
		void operator()(entt::registry& registry, float tick);
	};
	DEFINE_SYSTEM_TRAITS(TransformSystem, SystemType::Update)
}
