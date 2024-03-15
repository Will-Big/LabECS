#pragma once
#include "MainProcess.h"

namespace demo
{
	class DemoProcess final : public core::MainProcess
	{
	public:
		using MainProcess::MainProcess;

		~DemoProcess() override;
		void Initialize() override;

	protected:
		void Update() override;
		void Render() override;
	};

}

