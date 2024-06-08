#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Level.h"
#include "Systems/Camera.h"
#include "Resources/MeshesList.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Components/NameComponent.h"

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
	car->AddComponent<NameComponent>("Car");
	car->AddComponent<PositionComponent>(glm::vec3(-0.5f, 0, -1));
	car->AddComponent<RotationComponent>(glm::vec3(0, 0, 0));
	car->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	car->AddComponent<UboModelComponent>();
	car->AddComponent<MeshComponent>(formulaMesh);
	car->AddComponent<MaterialComponent>(formulaMaterial);

	auto vikingsRoomMesh = graphicsApi->CreateMesh(projectSettings->projectPath + meshes[0]);
	auto vikingsRoom = CreateRef<Entity>(ecs);
	vikingsRoom->AddComponent<NameComponent>("Room");
	vikingsRoom->AddComponent<PositionComponent>(glm::vec3(-0.5f, -100, -10));
	vikingsRoom->AddComponent<RotationComponent>(glm::vec3(0, 0, 90));
	vikingsRoom->AddComponent<ScaleComponent>(glm::vec3(140, 140, 140));
	vikingsRoom->AddComponent<RotationVelocityComponent>(glm::vec3(0, 0, 100));
	vikingsRoom->AddComponent<UboModelComponent>();
	vikingsRoom->AddComponent<MeshComponent>(vikingsRoomMesh);
	vikingsRoom->AddComponent<MaterialComponent>(vikingMaterial);

	auto cameraEntity = CreateRef<Entity>(ecs);
	cameraEntity->AddComponent<NameComponent>("Camera");
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
