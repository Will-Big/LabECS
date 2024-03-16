#pragma once
#include "EventManager.h"

namespace core
{
	// 이벤트를 구독하기 위해 상속해야 하는 클래스
	class EventListener
	{
	public:
		virtual ~EventListener()
		{
			EventManager::GetInstance()->RemoveListener(this);
		}

	protected:
		// EventManager::Subscribe 에서 사용되는 ListenerInfo 를 만들어주는 함수입니다.
		// 사용 예) eventManager->Subscribe("OnTest", CreateListenerInfo(&Class::MemberFunc))
		template <typename T> requires std::derived_from<T, EventListener>
		EventManager::ListenerInfo CreateListenerInfo(void (T::* func)(std::any))
		{
			return ListenerInfo(this, [this, func](std::any data) { (static_cast<T*>(this)->*func)(data); });
		}
	};
}

