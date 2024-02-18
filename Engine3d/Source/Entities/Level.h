#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Entity.h"
#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Model/Mesh.h"
#include "Components/Camera.h"
#include "Rendering/IRenderer.h"

class Level
{
public:
	Level(Ref<entt::registry> ecs, IRenderer* renderer, Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Ref<entt::registry> ecs;
	IRenderer* renderer;
	Rollback* rollback;

};

