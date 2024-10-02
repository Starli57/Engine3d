#pragma once

#include <vector>
#include <entt.hpp>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Defines/DllDefines.h"
#include "Entity.h"

class PROJECT_API Ecs
{
public:
	Ref<entt::registry> registry;
	std::vector<Ref<Entity>> allEntities;

	Ecs();
	~Ecs();

	Ref<Entity> CreateEntity();
	void DestroyEntiy(Ref<Entity> entity);

};

