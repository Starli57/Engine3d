#pragma once

#include <entt.hpp>

class Entity
{
public:
	Entity(entt::registry* ecs)
	{
		this->ecs = ecs;
		this->ecsEntity = ecs->create();
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return ecs->emplace<T>(ecsEntity, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& AddIfComponentNotExist(Args&&... args)
	{
		if (!HasComponent<T>()) return ecs->emplace<T>(ecsEntity, std::forward<Args>(args)...);
		else return GetComponent();
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		return ecs->emplace_or_replace<T>(ecsEntity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return ecs->get<T>(ecsEntity);
	}

	template<typename T>
	bool HasComponent()
	{
		//todo: test this
		return ecs->ctx().contains<T>(ecsEntity);
	}

	template<typename T>
	void RemoveComponent()
	{
		ecs->remove<T>(ecsEntity);
	}

	bool operator==(const Entity& other) const
	{
		return ecsEntity == other.ecsEntity;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	entt::registry* ecs;
	entt::entity ecsEntity;
};

