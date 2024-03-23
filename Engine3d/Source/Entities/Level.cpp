#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Level.h"
#include "SharedLib/Ref.h"
#include "Systems/Camera.h"
#include "Systems/MeshContainer.h"
#include "Resources/MeshesList.h"

Level::Level(Ref<entt::registry> ecs, Ref<ProjectSettigns> projectSettings, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback)
{
	this->ecs = ecs;
	this->projectSettings = projectSettings;
	this->assetDatabase = assetDatabase;
	this->graphicsApi = graphicsApi;
	this->rollback = new Rollback("Level", *rollback);
}

Level::~Level()
{
	rollback->Dispose();
}

void Level::LoadLevel()
{
	spdlog::info("Load level");

	auto texture = graphicsApi->CreateTexture(TextureId::formula1_Diffuse);
	assetDatabase->AddTexture(texture);

	//todo: make dispose for textures better
	rollback->Add([this]() { assetDatabase->RemoveTexture(TextureId::formula1_Diffuse); });

	auto material = CreateRef<Material>(texture);
	auto carMesh = graphicsApi->CreateMesh(projectSettings->projectPath + meshes[1]);
	auto car = CreateRef<Entity>(ecs);
	car->AddComponent<PositionComponent>(glm::vec3(-0.5f, 0, -1));
	car->AddComponent<RotationComponent>(glm::vec3(0, 0, 0));
	car->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	car->AddComponent<UboModelComponent>();
	car->AddComponent<MeshContainer>(carMesh, material);

	auto cameraEntity = CreateRef<Entity>(ecs);
	cameraEntity->AddComponent<PositionComponent>(glm::vec3(0, 1, 500));
	cameraEntity->AddComponent<RotationComponent>(glm::vec3(0, 0, 0));
	cameraEntity->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	cameraEntity->AddComponent<UboModelComponent>();
	cameraEntity->AddComponent<UboViewProjectionComponent>();

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");
	ecs->clear();
}
