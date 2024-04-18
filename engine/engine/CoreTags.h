#pragma once

#define DEFINE_TAG(class) \
	struct class \
	{ \
		static constexpr entt::id_type id = entt::type_hash<class>().value(); \
	} \

namespace core
{
	namespace tag
	{
		// built-in tag
		DEFINE_TAG(Untagged);
		DEFINE_TAG(Respawn);
		DEFINE_TAG(Finish);
		DEFINE_TAG(MainCamera);
		DEFINE_TAG(Player);
	}

	// 충돌 함수 콜백에 사용 (CollisionCallback 클래스 참조)
	struct Tag
	{
		entt::id_type id = tag::Untagged::id;
	};
}
