#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Entities/Mesh.h"
#include "AssetsDatabase.h"
#include "Systems/Camera.h"
#include "Systems/RotatorSystem.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Model/Vertex.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/Entity.h"
#include "EngineShared/ProjectSettings.h"
#include "EngineShared/Rollback/Rollback.h"

class Level
{
public:
	Level(Ref<Ecs> ecs, Ref<ProjectSettigns> projectSettings, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Ref<Ecs> ecs;
	Ref<AssetsDatabase> assetDatabase;
	Ref<ProjectSettigns> projectSettings;

	IGraphicsApi* graphicsApi;
	Rollback* rollback;

};

