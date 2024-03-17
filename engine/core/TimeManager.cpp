#include "pch.h"
#include "TimeManager.h"

core::TimeManager::TimeManager()
{
	// 현재 카운트
	QueryPerformanceCounter(&prevCount_);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&frequency_);
}

void core::TimeManager::Update()
{
	QueryPerformanceCounter(&curCount_);
	delta_ = static_cast<double>(curCount_.QuadPart - prevCount_.QuadPart) / static_cast<double>(frequency_.QuadPart);
	prevCount_ = curCount_;

	++Fps_;

	if (elapsed_ > 1.0) 
	{
		elapsed_ -= 1.0;
		lastFps_ = Fps_;
		Fps_ = 0;
	}
}
