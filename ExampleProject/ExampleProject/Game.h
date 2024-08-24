#pragma once

#include "EngineCore/Engine.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

class Game
{
public:
	Game(Ref<ProjectSettigns> projectSettings);
	~Game();

	void Run();

	Ref<Engine> engine;
	Ref<ProjectSettigns> projectSettings;

private:
	Ref<AssetsDatabase> assetDatabase;
	Ref<Ecs> ecs;
	IGraphicsApi* graphicsApi;

	void LoadLevel();
};

