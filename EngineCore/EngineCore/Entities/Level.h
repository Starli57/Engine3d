#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "EngineCore/AssetsDatabase.h"
#include "EngineCore/Systems/Camera.h"
#include "EngineCore/Systems/RotatorSystem.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Assets/Mesh.h"
#include "EngineCore/Vertex.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"
#include "EngineCore/Entity.h"
#include "EngineCore/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Components/MeshComponent.h"

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

};

