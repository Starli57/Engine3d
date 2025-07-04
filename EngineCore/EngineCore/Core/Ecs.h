#pragma once

#include <vector>
#include <entt.hpp>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Defines/DllDefines.h"
#include "Entity.h"

class Ecs
{
public:
	Ref<entt::registry> registry;
	std::vector<Ref<Entity>> allEntities;

	Ecs();
	~Ecs();

	Ref<Entity> CreateEntity();
	Ref<Entity> FindEntity(entt::entity entity);
	
	void DestroyEntity(const Ref<Entity>& entity);

};

