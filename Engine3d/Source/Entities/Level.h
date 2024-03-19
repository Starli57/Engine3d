#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Systems/Camera.h"
#include "Systems/MeshContainer.h"

#include "Entities/Mesh.h"
#include "AssetsDatabase.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Model/Vertex.h"
#include "SharedLib/Rollback/Rollback.h"
#include "SharedLib/Entity.h"
#include "SharedLib/ProjectSettings.h"
#include "SharedLib/Components/PositionComponent.h"
#include "SharedLib/Components/RotationComponent.h"
#include "SharedLib/Components/ScaleComponent.h"
#include "SharedLib/Components/UboModelComponent.h"

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

