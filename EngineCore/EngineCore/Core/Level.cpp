#include "EngineCore/Pch.h"
#include "Level.h"
#include <EngineCore/Serialization/EntitySerializer.h>

Level::Level(Ref<Ecs> ecs, Ref<ProjectSettigns> projectSettings, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback)
{
	this->ecs = ecs;
	this->projectSettings = projectSettings;
	this->assetDatabase = assetDatabase;
	this->graphicsApi = graphicsApi;
}

Level::~Level()
{
	spdlog::info("Dispose level");
	ecs.reset();
}

void Level::InstantiatePrefab(const std::filesystem::path& path)
{
	auto serializer = CreateRef<EntitySerializer>(projectSettings);
	serializer->InstantiatePrefab(ecs, path);
}
