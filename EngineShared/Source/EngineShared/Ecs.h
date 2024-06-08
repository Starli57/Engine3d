#pragma once

#include <vector>
#include <entt.hpp>

#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"

class Ecs
{
public:
	Ref<entt::registry> registry;

	Ecs();
	~Ecs();

	Ref<Entity> CreateEntity();
	void DestroyEntiy(Ref<Entity> entity);

private:
	std::vector<Ref<Entity>> allEntities;
};

