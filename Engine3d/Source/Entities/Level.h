#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "SharedLib/Rollback/Rollback.h"

#include "Entity.h"
#include "Model/Vertex.h"
#include "Entities/Mesh.h"
#include "Components/Camera.h"
#include "IGraphicsApi.h"
#include "AssetsDatabase.h"

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

