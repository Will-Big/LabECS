#pragma once
#include "ISystemCycle.h"

namespace core
{
	struct CoreManagers;

	class CORE_EXPORT SystemManager
	{
	public:
		SystemManager(CoreManagers& coreManager) : cores_(coreManager) {}

		template <IsSystem System>
		void AddSystem();

		template <IsSystem System>
		void RemoveSystem();

		void Update(float delta) const;
		void FixedUpdate(float delta) const;
		void Render() const;

		void Clear();

	private:
		CoreManagers& cores_;

		std::vector<std::shared_ptr<ISystem>> systems_;

		std::vector<IUpdatable*> updates_;
		std::vector<IFixedUpdatable*> fixes_;
		std::vector<IRenderable*> renders_;
	};

	template <IsSystem System>
	void SystemManager::AddSystem()
	{
		systems_.push_back(std::make_shared<System>(cores_));

		if constexpr (std::is_base_of_v<IUpdatable, System>)
		{
			updates_.push_back(static_cast<System*>(systems_.back().get()));
		}
		if constexpr (std::is_base_of_v<IFixedUpdatable, System>)
		{
			fixes_.push_back(static_cast<System*>(systems_.back().get()));
		}
		if constexpr (std::is_base_of_v<IRenderable, System>)
		{
			renders_.push_back(static_cast<System*>(systems_.back().get()));
		}
	}

	template <IsSystem System>
	void SystemManager::RemoveSystem()
	{
		// systems_ 에서 System 타입의 포인터를 찾고 제거
		System* targetSystem = nullptr;
		for (auto it = systems_.begin(); it != systems_.end(); ++it) 
		{
			if ((targetSystem = dynamic_cast<System*>(it->get())) != nullptr) 
			{
				systems_.erase(it);
				break; // 일치하는 첫 번째 시스템을 찾으면 반복을 중단
			}
		}

		if (!targetSystem) 
			return;

		if constexpr (std::is_base_of_v<IUpdatable, System>)
		{
			auto it = std::find(updates_.begin(), updates_.end(), targetSystem);
			if (it != updates_.end())
				updates_.erase(it);
		}

		if constexpr (std::is_base_of_v<IFixedUpdatable, System>)
		{
			auto it = std::find(fixes_.begin(), fixes_.end(), targetSystem);
			if (it != fixes_.end())
				fixes_.erase(it);
		}

		if constexpr (std::is_base_of_v<IRenderable, System>)
		{
			auto it = std::find(renders_.begin(), renders_.end(), targetSystem);
			if (it != renders_.end())
				renders_.erase(it);
		}
	}
}

