﻿#pragma once

#include "Entity.h"

namespace core
{
	class Scene;
	class ICollisionHandler;

	// 시스템 시작
	struct OnStartSystem
	{
		Scene* scene = nullptr;

		OnStartSystem(Scene& scene) : scene(&scene) {}
	};

	// 시스템 종료
	struct OnFinishSystem
	{
		Scene* scene = nullptr;

		OnFinishSystem(Scene& scene) : scene(&scene) {}
	};

	// 엔티티 생성
	struct OnCreateEntity
	{
		Entity entity;

		OnCreateEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnCreateEntity(const Entity& entity) : entity(entity) {}
	};

	// 엔티티 삭제
	struct OnDestroyEntity
	{
		Entity entity;

		OnDestroyEntity(entt::entity handle, entt::registry& registry) : entity(handle, registry) {}
		OnDestroyEntity(const Entity& entity) : entity(entity) {}
	};


	/*!
	 * @code
	 * 충돌을 처리하는 시스템이 CollisionCallback 클래스에게 발행하는 이벤트
	 * 시스템 생성자나 OnStartSystem 시점에서 사용, ex)
	 * entt::dispatcher->trigger<core::OnRegisterCollisionHandler>({ entt::type_hash<ComponentClass>(), this });
	 * @endcode
	 * id(타입) 와 관련된 모든 충돌을 이 구조체로 이벤트를 연결한 클래스가 받음, CollisionCallback 클래스 참조
	 */
	struct OnRegisterCollisionHandler
	{
		entt::id_type id;
		ICollisionHandler* handler;

		OnRegisterCollisionHandler(const entt::id_type& id, ICollisionHandler* handler)
			: id(id), handler(handler) {}
	};


	/*!
	 * OnRegisterCollisionHandler 와 사용법은 같음
	 */
	struct OnRemoveCollisionHandler
	{
		entt::id_type id;
		ICollisionHandler* handler;

		OnRemoveCollisionHandler(const entt::id_type& id, ICollisionHandler* handler)
			: id(id), handler(handler) {}
	};
}
