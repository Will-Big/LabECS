#pragma once

namespace core
{
	class ISystem;

	using EntityId = size_t;        // Entity 개별 Id 값
    using ComponentId = uint32_t;   // Component 클래스의 HashId

    using EventType = std::string;
    using EventCallback = std::function<void(std::any)>;

    // compile time FNV-1a
    constexpr ComponentId Hash64_CT(const char* str, size_t n, uint64_t basis = UINT64_C(14695981039346656037))
    {
        return n == 0 ? basis : Hash64_CT(str + 1, n - 1, (basis ^ str[0]) * UINT64_C(1099511628211));
    }

    // compile time FNV-1a
    constexpr ComponentId Hash32_CT(const char* str, size_t n, uint32_t basis = UINT32_C(2166136261))
	{
        return n == 0 ? basis : Hash32_CT(str + 1, n - 1, (basis ^ str[0]) * UINT32_C(16777619));
    }

    template< ComponentId id >
    constexpr ComponentId typeid_helper()
	{
        return id;
    }

    template <typename T>
    concept IsComponent = requires
	{
        { T::componentId } -> std::convertible_to<ComponentId>;
    };

    template <typename T>
    concept IsSystem = std::is_base_of_v<ISystem, T>;
}

// 컴파일 타임 타입 ID
// using, typedef, nested, namespace 을 지원하지 않으므로 유일성이 보장되지 않을 수 있음
#define TYPEID( T ) typeid_helper< Hash32_CT( #T, sizeof( #T ) - 1 ) >()

// 컴포넌트 compile-time-type-id 생성
#define COMPONENT_INFO( T ) static constexpr ComponentId componentId = TYPEID( T );
