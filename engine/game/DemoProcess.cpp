﻿#include "pch.h"
#include "DemoProcess.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "ComponentPool.h"
#include "CoreManagers.h"
#include "EventManager.h"
#include "Transform.h"
#include "Parent.h"
#include "SystemManager.h"
#include "Tester.h"
#include "TestSystem.h"

using namespace core;

demo::DemoProcess::~DemoProcess()
{
}

void demo::DemoProcess::Initialize()
{
	MainProcess::Initialize();

	cores_.component->AddComponent<Transform>(1);
	cores_.component->AddComponent<Tester>(1).a1 = -1000;

	//cores_.component->AddComponent<Transform>(2);
	cores_.component->AddComponent<Tester>(2);

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
