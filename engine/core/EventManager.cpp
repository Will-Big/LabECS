#include "pch.h"
#include "EventManager.h"

void core::EventManager::Update(float deltaTime)
{
	// 실행할 이벤트를 별도의 목록으로 이동
	std::vector<DelayedEvent> eventsToExecute;

	auto it = std::ranges::remove_if(
		m_DelayedEvents, [deltaTime, &eventsToExecute](DelayedEvent& event)
		{
			event.remainingTime -= deltaTime;
			if (event.remainingTime <= 0)
			{
				eventsToExecute.push_back(std::move(event));
				return true; // 원본 목록에서 제거
			}
			return false;
		}).begin();

	m_DelayedEvents.erase(it, m_DelayedEvents.end());

	// 지연 이벤트 실행
	for (auto& event : eventsToExecute)
		event.event();

	// 즉시 실행 이벤트 처리
	for (auto& event : m_ImmediateEvents)
		event(); // 이벤트 실행

	m_ImmediateEvents.clear(); // 처리된 즉시 실행 이벤트 리스트 클리어
}

void core::EventManager::Subscribe(const EventType& type, const ListenerInfo& listenerInfo)
{
	m_Listeners[type].push_back(listenerInfo);
}

void core::EventManager::ScheduleEvent(const EventType& type, std::any data, float delay)
{
	if (delay <= 0)
		m_ImmediateEvents.emplace_back([=]() { PublishImmediate(type, data); });
	else
		m_DelayedEvents.emplace_back(delay, [=]() { PublishImmediate(type, data); });
}

void core::EventManager::PublishImmediate(const EventType& type, std::any data)
{
	if (m_Listeners.find(type) != m_Listeners.end())
		for (auto& listener : m_Listeners[type])
			listener.callback(data);
}

void core::EventManager::RemoveListener(EventListener* handler)
{
	for (auto& listenerInfos : m_Listeners | std::views::values)
	{
		auto it = std::ranges::remove_if(listenerInfos,
		                                 [handler](const ListenerInfo& info) { return info.listener == handler; }).begin();

		listenerInfos.erase(it, listenerInfos.end());
	}
}
