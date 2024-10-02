#include "EngineCore/Pch.h"
#include "World.h"
#include <EngineCore/Serialization/EntitySerializer.h>

World::World(Ref<Ecs> ecs, Ref<ProjectSettigns> projectSettings)
{
	this->ecs = ecs;
	this->projectSettings = projectSettings;
}

World::~World()
{
	spdlog::info("Dispose level");
	ecs.reset();
}

void World::InstantiatePrefab(const std::filesystem::path& path)
{
	auto serializer = CreateRef<EntitySerializer>(projectSettings);
	serializer->InstantiatePrefab(ecs, path);
}
