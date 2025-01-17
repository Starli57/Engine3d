#pragma once

#include <filesystem>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/ProjectSettings.h"

class World
{
public:
	World(Ref<Ecs> ecs, Ref<ProjectSettings> projectSettings);
	~World();

private:
	Ref<Ecs> ecs;
	Ref<ProjectSettings> projectSettings;

};

