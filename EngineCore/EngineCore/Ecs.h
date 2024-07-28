#pragma once

#include <vector>
#include <entt.hpp>

#include "EngineCore/Ref.h"
#include "Entity.h"

class Ecs
{
public:
	Ref<entt::registry> registry;
	std::vector<Ref<Entity>> allEntities;

	Ecs();
	~Ecs();

	Ref<Entity> CreateEntity();
	void DestroyEntiy(Ref<Entity> entity);

};

