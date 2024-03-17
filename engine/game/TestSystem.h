#pragma once
#include "ISystem.h"

namespace demo
{
	class TestSystem1 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem2 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem3 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem4 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem5 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem6 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};

	class TestSystem7 : public core::ISystem, public core::IUpdatable, public core::IFixedUpdatable, public core::IRenderable
	{
	public:
		using ISystem::ISystem;

		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() override;
	};
}

