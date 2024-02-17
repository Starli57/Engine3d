#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Entity.h"
#include "Architecture/Rollback/Rollback.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Model/Mesh.h"
#include "Components/Camera.h"
#include <entt.hpp>

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

