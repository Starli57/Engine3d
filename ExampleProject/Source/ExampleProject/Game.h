#pragma once

#include "EngineCore/Engine.h"

#include "EngineCore/Ref.h"
#include "EngineCore/ProjectSettings.h"

class Game
{
public:
	Game(Ref<ProjectSettigns> projectSettings);
	~Game();

	void Run();

	Ref<Engine> engine;
	Ref<ProjectSettigns> projectSettings;
};

