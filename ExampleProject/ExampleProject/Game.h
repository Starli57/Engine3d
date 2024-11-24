#pragma once

#include "Editor/Editor.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/ResourcesManager.h"

class Game
{
public:
	Game(Ref<ProjectSettigns> projectSettings);
	~Game();

	void Run();
private:
	Ref<Editor> editor;
	Ref<Engine> engine;

	Ref<ProjectSettigns> projectSettings;
	Ref<AssetsDatabase> assetDatabase;
	Ref<ResourcesManager> resourcesManager;
	Ref<Ecs> ecs;
	IGraphicsApi* graphicsApi;

	void LoadResources();
};

