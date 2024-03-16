#include "pch.h"
#include "DemoProcess.h"

#include "Transform.h"
#include "PoolManager.h"
#include "EntityManager.h"
#include "ComponentPool.h"

core::PoolManager pm;
core::EntityManager em;

using namespace core;

struct Tester
{
	COMPONENT_INFO(Tester)
};

demo::DemoProcess::~DemoProcess()
{
}

void demo::DemoProcess::Initialize()
{
	MainProcess::Initialize();

	auto& t1 = pm.GetPool<core::Transform>();
	auto& t2 = pm.GetPool<Tester>();
}

void demo::DemoProcess::Update()
{
	MainProcess::Update();
}

void demo::DemoProcess::Render()
{
	MainProcess::Render();
}
