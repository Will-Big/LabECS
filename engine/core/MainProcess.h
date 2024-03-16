#pragma once
#include "CoreManagers.h"

namespace core
{

	// 게임의 사이클을 관장하는 메인 클래스
	class CORE_EXPORT MainProcess
	{
	public:
		MainProcess(HINSTANCE hInst, std::string_view title, uint32_t width, uint32_t height, bool isLauncher);
		virtual ~MainProcess();

		virtual void Initialize();
		void Loop();

	protected:
		virtual void Update();
		virtual void Render();

		CoreManagers cores_;
		float time_ = 0.f;

	private:
		HWND hwnd_;
		std::string title_;
		uint32_t width_;
		uint32_t height_;
	};
}
