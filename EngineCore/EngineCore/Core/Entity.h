#pragma once

#include <entt.hpp>
#include "EngineCore/Core/Ref.h"

class Entity
{
public:
	entt::entity GetEntity() const { return entity; }

	Entity(const Ref<entt::registry>& registry)
	{
		this->registry = registry;
		this->entity = registry->create();
	}

	~Entity()
	{

	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return registry->emplace<T>(entity, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& AddComponentIfNotExist(Args&&... args)
	{
		if (!HasComponent<T>()) return registry->emplace<T>(entity, std::forward<Args>(args)...);
		else return GetComponent<T>();
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		return registry->emplace_or_replace<T>(entity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return registry->get<T>(entity);
	}

	template<typename T>
	bool HasComponent() const
	{
		return registry->any_of<T>(entity);
	}

	template<typename T>
	void RemoveComponent() const
	{
		registry->remove<T>(entity);
	}

	bool operator==(const Entity& other) const
	{
		return entity == other.entity;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	Ref<entt::registry> registry;
	entt::entity entity;
};

