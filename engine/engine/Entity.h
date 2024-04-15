#pragma once

namespace core
{
	class Entity
	{
	public:
		Entity(entt::entity handle, entt::registry& registry) : _handle(handle), _registry(registry) {}

		template <typename T, typename... Args>
		T& Emplace(Args&&... args);

		template <typename T, typename... Args>
		T& Replace(Args&&... args);

		template <typename T>
		T& Get() const;

		template<typename... Args>
		bool HasAllOf() const;

		template<typename... Args>
		bool HasAnyOf() const;

		template<typename... Args>
		void Remove() const;

		void Destroy() const { _registry.destroy(_handle); }

		void SetParent(const core::Entity& entity);
		std::vector<Entity> GetChildren() const;

		bool IsAncestorOf(core::Entity entity) const;
		bool IsDescendantOf(const Entity& entity) const;

		const entt::entity& GetHandle() const { return _handle; }
		//uint32_t GetHandle() const { return static_cast<uint32_t>(_handle); }

		operator uint32_t () const { return static_cast<uint32_t>(_handle); }
		operator entt::entity() const { return _handle; }
		operator bool() const { return (_handle != entt::null) && _registry.valid(_handle); }

		Entity& operator=(const Entity& other) { if (this == &other) { return *this; } _handle = other._handle; return *this; }
		bool operator==(const Entity& other) const { return _handle == other._handle && &_registry == &other._registry; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity _handle = entt::null;
		entt::registry& _registry;
	};

	template <typename T, typename... Args>
	T& Entity::Emplace(Args&&... args)
	{
		return _registry.emplace<T>(_handle, std::forward<Args>(args)...);
	}

	template <typename T, typename ... Args>
	T& Entity::Replace(Args&&... args)
	{
		return _registry.replace<T>(_handle, std::forward<Args>(args)...);
	}

	template <typename T>
	T& Entity::Get() const
	{
		return _registry.get<T>(_handle);
	}

	template <typename... Args>
	bool Entity::HasAllOf() const
	{
		return _registry.all_of<Args...>(_handle);
	}

	template <typename ... Args>
	bool Entity::HasAnyOf() const
	{
		return _registry.any_of<Args...>(_handle);
	}

	template <typename... Args>
	void Entity::Remove() const
	{
		_registry.remove<Args...>(_handle);
	}
}


