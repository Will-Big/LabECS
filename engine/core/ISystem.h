#pragma once

#include "ISystemCycle.h"
#include "CoreManagers.h"

namespace core
{
	class ISystem
	{
	public:
		ISystem(CoreManagers& coreManagers) : cores_(coreManagers) {}
		virtual ~ISystem() = default;

	protected:
		CoreManagers& cores_;
	};
}

