#include "pch.h"
#include "SystemManager.h"

void core::SystemManager::Update(float delta) const
{
	for (const auto& update : updates_)
		update->Update(delta);
}

void core::SystemManager::FixedUpdate(float delta) const
{
	for (const auto& fixed : fixes_)
		fixed->FixedUpdate(delta);
}

void core::SystemManager::Render() const
{
	for (const auto& render : renders_)
		render->Render();
}

void core::SystemManager::Clear()
{
	systems_.clear();

	updates_.clear();
	fixes_.clear();
	renders_.clear();
}
