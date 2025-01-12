#include "EngineCore/Pch.h"
#include "World.h"
#include <EngineCore/Serialization/EntitySerializer.h>

World::World(Ref<Ecs> ecs, Ref<ProjectSettings> projectSettings)
{
	this->ecs = ecs;
	this->projectSettings = projectSettings;
}

World::~World()
{
	spdlog::info("Dispose level");
	ecs.reset();
}
