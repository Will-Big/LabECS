#include "pch.h"
#include "EventManager.h"

void core::EventManager::Update(float deltaTime)
{
	// 이번 프레임에 실행할 이벤트 목록
	std::vector<DelayedEvent> eventsToExecute;

	// 실행할 이벤트를 원본 목록에서 제거
	auto it = std::ranges::remove_if(
		delayedEvents_, [deltaTime, &eventsToExecute](DelayedEvent& event)
		{
			event.remainingTime_ -= deltaTime;
			if (event.remainingTime_ <= 0)
			{
				// 실행할 이벤트를 별도의 목록으로 이동
				eventsToExecute.push_back(std::move(event));
				return true; 
			}
			return false;
		}).begin();

	delayedEvents_.erase(it, delayedEvents_.end());

	// 지연 이벤트 실행
	for (auto& event : eventsToExecute)
		event.event_();
}

void core::EventManager::Subscribe(const EventType& type, const ListenerInfo& listenerInfo)
{
	listeners_[type].push_back(listenerInfo);
}

void core::EventManager::ScheduleEvent(const EventType& type, std::any data, float delay)
{
	delayedEvents_.emplace_back(delay, [=]() { PublishImmediate(type, data); });
}

void core::EventManager::PublishImmediate(const EventType& type, std::any data)
{
	if (listeners_.find(type) != listeners_.end())
		for (auto& listener : listeners_[type])
			listener.callback_(data);
}

void core::EventManager::RemoveListener(EventListener* handler)
{
	for (auto& listenerInfos : listeners_ | std::views::values)
	{
		auto it = std::ranges::remove_if(listenerInfos,
		                                 [handler](const ListenerInfo& info) { return info.listener_ == handler; }).begin();

		listenerInfos.erase(it, listenerInfos.end());
	}
}
