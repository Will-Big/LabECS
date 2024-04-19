#pragma once

#define DEFINE_TAG(name) \
	struct name \
	{ \
		static constexpr entt::id_type id = entt::type_hash<name>().value(); \
	} \

#define DEFINE_LAYER(name, layerIndex) \
	struct name \
	{ \
		static constexpr entt::id_type id = entt::type_hash<name>().value(); \
		static constexpr uint32_t index = layerIndex; \
	} \

namespace tag
{
	// built-in tag
	DEFINE_TAG(Untagged);
	DEFINE_TAG(Respawn);
	DEFINE_TAG(Finish);
	DEFINE_TAG(MainCamera);
	DEFINE_TAG(Player);
}

namespace layer
{
	// built-in layer
	DEFINE_LAYER(Default, 0);
}

namespace core
{
	// 충돌 함수 콜백에 사용 (CollisionCallback 클래스 참조)
	struct Tag
	{
		entt::id_type id = tag::Untagged::id;
	};

	// 충돌 판정에 사용 (PhysicsScene 클래스 참조)
	struct Layer
	{
		uint32_t id = layer::Default::id;
	};
}