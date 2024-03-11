#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Entity.h"
#include "Entities/Mesh.h"
#include "AssetsDatabase.h"
#include "SharedLib/ProjectSettings.h"
#include "Components/Camera.h"
#include "Components/Rotator.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Model/Vertex.h"
#include "SharedLib/Rollback/Rollback.h"

class Level
{
public:
	Level(Ref<entt::registry> ecs, Ref<ProjectSettigns> projectSettings, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Ref<entt::registry> ecs;
	Ref<AssetsDatabase> assetDatabase;
	Ref<ProjectSettigns> projectSettings;

	IGraphicsApi* graphicsApi;
	Rollback* rollback;

};

