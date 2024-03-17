#pragma once
#include "ISystem.h"
#include "EventListener.h"

namespace core
{
	class InputSystem : public ISystem, public IUpdatable, public EventListener
	{
	public:
		using ISystem::ISystem;
		~InputSystem() override = default;

		void Update(float deltaTime) override;
	};

}
