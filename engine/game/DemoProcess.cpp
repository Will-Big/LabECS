#include "pch.h"
#include "DemoProcess.h"

#include "PoolManager.h"
#include "EntityManager.h"
#include "ComponentPool.h"
#include "CoreManagers.h"
#include "EventManager.h"
#include "Transform.h"
#include "Parent.h"

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

	auto e1 = cores_.entity->AddEntity();
	auto e2 = cores_.entity->AddEntity();
	auto e3 = cores_.entity->AddEntity();
	auto e4 = cores_.entity->AddEntity();

	cores_.pool->AddComponent<Tester>(e1).a1 = 10;
	cores_.pool->AddComponent<Tester>(e2).a1 = 100;
	cores_.pool->AddComponent<Tester>(e3).a1 = 1000;
	cores_.pool->AddComponent<Tester>(e4).a1 = 10000;

	cores_.pool->AddComponent<Parent>(e2).parentId = e1;
	cores_.pool->AddComponent<Parent>(e3).parentId = e2;
	cores_.pool->AddComponent<Parent>(e4).parentId = e3;

	auto c1 = cores_.entity->GetChildren(e1, cores_.pool.get());
	auto p1 = cores_.entity->GetParent(e3, cores_.pool.get());

	cores_.entity->RemoveEntity(e1, cores_.pool.get());
}

void demo::DemoProcess::Update()
{
	MainProcess::Update();
}

void demo::DemoProcess::Render()
{
	MainProcess::Render();
}
