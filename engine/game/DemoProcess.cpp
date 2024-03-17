#include "pch.h"
#include "DemoProcess.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "ComponentPool.h"
#include "CoreManagers.h"
#include "EventManager.h"
#include "Transform.h"
#include "Parent.h"
#include "SystemManager.h"
#include "TestSystem.h"

using namespace core;

struct Tester
{
	COMPONENT_INFO(Tester)

	int a1 = 0;
	int a2 = 0;
};

demo::DemoProcess::~DemoProcess()
{
}

void demo::DemoProcess::Initialize()
{
	MainProcess::Initialize();

	cores_.component->AddComponent<Transform>(1);

	system_->AddSystem<TestSystem1>();
	system_->AddSystem<TestSystem2>();
	system_->AddSystem<TestSystem3>();
	system_->AddSystem<TestSystem4>();
	system_->AddSystem<TestSystem5>();
	system_->AddSystem<TestSystem6>();
	system_->AddSystem<TestSystem7>();
}

void demo::DemoProcess::Update()
{
	MainProcess::Update();
}

void demo::DemoProcess::Render()
{
	MainProcess::Render();
}
