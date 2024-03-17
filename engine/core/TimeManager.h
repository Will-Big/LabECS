#pragma once

namespace core
{
	class TimeManager
	{
	public:
		TimeManager();
		~TimeManager() = default;

		void Update();

		float GetDeltaTime() const { return static_cast<float>(delta_); }
		uint32_t GetFPS() const { return lastFps_; }

	private:
		LARGE_INTEGER curCount_{};
		LARGE_INTEGER prevCount_{};
		LARGE_INTEGER frequency_{};

		double delta_ = 0;
		double elapsed_ = 0;
		uint32_t lastFps_ = 0;
		uint32_t Fps_ = 0;
	};
}


