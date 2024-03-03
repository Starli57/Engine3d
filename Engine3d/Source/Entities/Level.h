#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Entity.h"
#include "Components/Camera.h"
#include "AssetsDatabase.h"

#include "SharedLib/Rollback/Rollback.h"

#include "Renderer/Model/Vertex.h"
#include "Renderer/Entities/Mesh.h"
#include "Renderer/Entities/Material.h"
#include "Renderer/IGraphicsApi.h"

class Level
{
public:
	Level(Ref<entt::registry> ecs, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Ref<entt::registry> ecs;
	Ref<AssetsDatabase> assetDatabase;

	IGraphicsApi* graphicsApi;
	Rollback* rollback;

};

