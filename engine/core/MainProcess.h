#pragma once
#include "CoreManagers.h"

namespace core
{
	class SystemManager;
	class TimeManager;

	// 게임의 사이클을 관장하는 메인 클래스
	class CORE_API MainProcess
	{
	public:
		MainProcess(HINSTANCE hInst, std::string_view title, uint32_t width, uint32_t height, bool isLauncher);
		virtual ~MainProcess();

		virtual void Initialize();
		void Loop();

	protected:
		virtual void Update();
		virtual void Render();

		std::shared_ptr<SystemManager> system_;

		CoreManagers cores_;
		float delta_ = 0.f;

	private:
		std::shared_ptr<TimeManager> time_;

		HWND hwnd_;
		std::string title_;
		uint32_t width_;
		uint32_t height_;
	};
}
