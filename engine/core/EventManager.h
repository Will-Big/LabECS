#pragma once

namespace core
{
	class EventListener;

	// 이벤트를 관리하는 클래스
	class EventManager
	{
		// EventListener 와 실행할 콜백 함수
		struct ListenerInfo
		{
			EventListener* listener_;
			EventCallback callback_;

			ListenerInfo(EventListener* handler, EventCallback callback)
				: listener_(handler), callback_(std::move(callback)) {}
		};

		// 이벤트와 그 남은 실행 시간을 관리할 구조체
		struct DelayedEvent
		{
			float remainingTime_; // 이벤트 실행까지 남은 시간
			std::function<void()> event_;

			DelayedEvent(float delay, std::function<void()> event)
				: remainingTime_(delay), event_(std::move(event)) {}
		};

	public:
		// 예약된 이벤트를 실행합니다.
		void Update(float deltaTime);

		// 이벤트를 구독합니다.
		void Subscribe(const EventType& type, const ListenerInfo& listenerInfo);

		// delay 초 이후 발생할 이벤트를 예약합니다.
		// 예약된 이벤트는 프레임의 마지막에 실행됩니다.
		void ScheduleEvent(const EventType& type, std::any data = std::any{}, float delay = 0);

		// 이벤트를 즉시 발행합니다.
		// 발행된 이벤트는 호출 시점에 실행됩니다.
		void PublishImmediate(const EventType& type, std::any data = std::any{});

		// listeners_ 에 저장된 EventListener 를 삭제합니다.
		// EventListener 의 소멸자에서 자동으로 호출됩니다.
		void RemoveListener(EventListener* handler);

	private:
		std::unordered_map<EventType, std::vector<ListenerInfo>> listeners_;
		std::vector<DelayedEvent> delayedEvents_;

		friend class EventListener;
	};
}

