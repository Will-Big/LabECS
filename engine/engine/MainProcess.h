#pragma once

namespace engine
{
	struct ProcessInfo
	{
		std::string name = "Hazel";
		HINSTANCE hInstance;
		uint32_t width = 1600, height = 900;
		bool enableImGui = true;
	};


	class MainProcess
	{
	public:
		MainProcess(const ProcessInfo& info);
	};
}

