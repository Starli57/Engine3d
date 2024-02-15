#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Entity.h"
#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Model/Mesh.h"
#include "Components/Camera.h"

#include <entt.hpp>

class World
{
public:
	World(entt::registry& ecs, Rollback* rollback);
	~World();

	void LoadLevel();
	void UnloadLevel();

private:
	entt::registry* ecs;
	Rollback* rollback;

};

