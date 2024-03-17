#include "pch.h"
#include "TestSystem.h"

#include "ComponentManager.h"
#include "Transform.h"

using namespace core;

#define TASK \
	volatile int task = 0; \
	for (int i = 0; i < 1000000; ++i) \
		++task \

void demo::TestSystem1::Update(float deltaTime)
{
	cores_.component->ForEachComponent<Transform>([](Transform& transform)
		{
			transform.a = -10;
			transform.b = 10;
		});


	for(auto it = cores_.component->begin<Transform>(); it != cores_.component->end<Transform>(); ++it)
	{
		auto eid = cores_.component->GetEntityId<Transform>(it);
		int i = 0;
	}
}

void demo::TestSystem1::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem1::Render()
{
	TASK;
}

void demo::TestSystem2::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem2::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem2::Render()
{
	TASK;
}

void demo::TestSystem3::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem3::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem3::Render()
{
	TASK;
}

void demo::TestSystem4::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem4::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem4::Render()
{
	TASK;
}

void demo::TestSystem5::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem5::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem5::Render()
{
	TASK;
}

void demo::TestSystem6::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem6::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem6::Render()
{
	TASK;
}

void demo::TestSystem7::Update(float deltaTime)
{
	TASK;
}

void demo::TestSystem7::FixedUpdate(float deltaTime)
{
	TASK;
}

void demo::TestSystem7::Render()
{
	TASK;
}
