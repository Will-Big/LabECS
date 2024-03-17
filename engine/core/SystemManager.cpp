#include "pch.h"
#include "SystemManager.h"
#include "ISystemCycle.h"

core::SystemManager::SystemManager(CoreManagers& coreManager)
	: cores_(coreManager)
{
}


#ifndef CORE_THREAD
void core::SystemManager::Update(float delta)
{
	for (const auto& update : updates_)
		update->Update(delta);
}

void core::SystemManager::FixedUpdate(float delta)
{
	for (const auto& fixed : fixes_)
		fixed->FixedUpdate(delta);
}

void core::SystemManager::Render()
{
	for (const auto& render : renders_)
		render->Render();
}
#else
void core::SystemManager::Update(float delta)
{
	std::vector<std::future<void>> futures;
	for (const auto& update : updates_)
	{
		auto future = threadPool_.submit_task([update, delta]()
			{
				update->Update(delta);
			});
		futures.push_back(std::move(future));
	}

	for (auto& future : futures)
	{
		future.get();
	}
}

void core::SystemManager::FixedUpdate(float delta)
{
	std::vector<std::future<void>> futures;
	for (const auto& fixed : fixes_)
	{
		auto future = threadPool_.submit_task([fixed, delta]()
			{
				fixed->FixedUpdate(delta);
			});
		futures.push_back(std::move(future));
	}

	for (auto& future : futures)
	{
		future.get();
	}
}

void core::SystemManager::Render()
{
	std::vector<std::future<void>> futures;
	for (const auto& render : renders_)
	{
		auto future = threadPool_.submit_task([render]()
			{
				render->Render();
			});
		futures.push_back(std::move(future));
	}

	for (auto& future : futures)
	{
		future.get();
	}
}
#endif

void core::SystemManager::Clear()
{
	systems_.clear();

	updates_.clear();
	fixes_.clear();
	renders_.clear();
}
