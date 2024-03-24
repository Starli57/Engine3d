#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Level.h"
#include "SharedLib/Ref.h"
#include "Systems/Camera.h"
#include "Systems/MeshComponent.h"
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

	auto vikingTexture = graphicsApi->CreateTexture(TextureId::viking_room);
	assetDatabase->AddTexture(vikingTexture);
	rollback->Add([this]() { assetDatabase->RemoveTexture(TextureId::viking_room); });

	auto formulaDefuseTexture = graphicsApi->CreateTexture(TextureId::formula1_Diffuse);
	assetDatabase->AddTexture(formulaDefuseTexture);
	//todo: make dispose for textures better
	rollback->Add([this]() { assetDatabase->RemoveTexture(TextureId::formula1_Diffuse); });

	auto vikingMaterial = CreateRef<Material>(vikingTexture);
	auto formulaMaterial = CreateRef<Material>(formulaDefuseTexture);

	auto formulaMesh = graphicsApi->CreateMesh(projectSettings->projectPath + meshes[1]);
	auto car = CreateRef<Entity>(ecs);
	car->AddComponent<PositionComponent>(glm::vec3(-0.5f, 0, -1));
	car->AddComponent<RotationComponent>(glm::vec3(0, 0, 0));
	car->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	car->AddComponent<UboModelComponent>();
	car->AddComponent<MeshComponent>(formulaMesh, formulaMaterial);

	auto car2 = CreateRef<Entity>(ecs);
	car2->AddComponent<PositionComponent>(glm::vec3(-0.5f, -100, -10));
	car2->AddComponent<RotationComponent>(glm::vec3(0, 0, 180));
	car2->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	car2->AddComponent<RotationVelocityComponent>(glm::vec3(0, 0, 100));
	car2->AddComponent<UboModelComponent>();
	car2->AddComponent<MeshComponent>(formulaMesh, vikingMaterial);

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
