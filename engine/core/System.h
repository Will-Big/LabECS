#pragma once

#include "ISystemCycle.h"
#include "CoreManagers.h"

namespace core
{
	class System
	{
	public:
		System(CoreManagers& coreManagers);
		virtual ~System() = default;

	protected:
		CoreManagers& coreManagers_;
	};
}

