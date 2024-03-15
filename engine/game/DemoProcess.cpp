#include "pch.h"
#include "DemoProcess.h"

#include "Transform.h"
#include "PoolManager.h"
#include "EntityManager.h"
#include "ComponentPool.h"

core::PoolManager pm;
core::EntityManager em;

demo::DemoProcess::~DemoProcess()
{
}

void demo::DemoProcess::Initialize()
{
	MainProcess::Initialize();
}

void demo::DemoProcess::Update()
{
	MainProcess::Update();
}

void demo::DemoProcess::Render()
{
	MainProcess::Render();
}
