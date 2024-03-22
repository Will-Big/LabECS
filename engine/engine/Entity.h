#pragma once

namespace engine
{
	class Entity
	{
	public:
		Entity(entt::entity handle, entt::registry& registry) : _handle(handle), _registry(registry) {}

		template <typename T, typename... Args>
		T& Emplace(Args&&... args);

		template <typename T>
		T& Get() const;

		template<typename... Args>
		bool AllOf() const;

		template<typename... Args>
		bool AnyOf() const;

		template<typename... Args>
		void Remove() const;

		void SetParent(Entity entity);

		bool IsAncestorOf(Entity entity);

		operator uint32_t () const { return static_cast<uint32_t>(_handle); }
		operator entt::entity() const { return _handle; }
		operator bool() const { return (_handle != entt::null) && _registry.valid(_handle); }

	private:
		entt::entity _handle = entt::null;
		entt::registry& _registry;
	};

	template <typename T, typename... Args>
	T& Entity::Emplace(Args&&... args)
	{
		return _registry.emplace<T>(_handle, std::forward<Args>(args)...);
	}

	template <typename T>
	T& Entity::Get() const
	{
		return _registry.get<T>(_handle);
	}

	template <typename... Args>
	bool Entity::AllOf() const
	{
		return _registry.all_of<Args>(_handle);
	}

	template <typename ... Args>
	bool Entity::AnyOf() const
	{
		return _registry.any_of<Args>(_handle);
	}

	template <typename... Args>
	void Entity::Remove() const
	{
		_registry.remove<Args>(_handle);
	}
}


