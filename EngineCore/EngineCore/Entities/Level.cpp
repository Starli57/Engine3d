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
	UnloadLevel();
}

void Level::LoadLevel()
{
	spdlog::info("Load level");

	//textures
	auto vikingTexturePath = assetDatabase->texturesPaths.find("viking_room.png");
	auto vikingTexture = graphicsApi->CreateTexture(vikingTexturePath->second);
	auto vikignTextureIndex = assetDatabase->AddTexture(vikingTexture);

	auto formulaTexturePath = assetDatabase->texturesPaths.find("formula1_DefaultMaterial_Diffuse.png");
	auto formulaDefuseTexture = graphicsApi->CreateTexture(formulaTexturePath->second);
	auto formulaTextureIndex = assetDatabase->AddTexture(formulaDefuseTexture);

	//materials
	std::string albedoOpaquePipelineId = "albedoOpaque";
	auto vikingMaterial = graphicsApi->CreateMaterial(albedoOpaquePipelineId);
	vikingMaterial->SetAlbedoTexture(vikignTextureIndex);
	auto vikingMaterialIndex = assetDatabase->AddMaterial(vikingMaterial);

	auto formulaMaterial = graphicsApi->CreateMaterial(albedoOpaquePipelineId);
	formulaMaterial->SetAlbedoTexture(formulaTextureIndex);
	auto formulaMaterialIndex = assetDatabase->AddMaterial(formulaMaterial);

	std::string vertexColorOpaquePipelineId = "vertexColorOpaque";
	auto formulaColorMaterial = graphicsApi->CreateMaterial(vertexColorOpaquePipelineId);
	auto formulaColorMaterialIndex = assetDatabase->AddMaterial(formulaColorMaterial);

	//meshes
	auto formulaMeshPath = assetDatabase->meshesPaths.find("Formula_1_mesh.obj");
	auto formulaMesh = graphicsApi->CreateMesh(formulaMeshPath->second);
	auto formulaMeshIndex = assetDatabase->AddMesh(formulaMesh);

	auto vikingsRoomMeshPath = assetDatabase->meshesPaths.find("viking_room.obj");
	auto vikingsRoomMesh = graphicsApi->CreateMesh(vikingsRoomMeshPath->second);
	auto vikingsRoomMeshIndex = assetDatabase->AddMesh(vikingsRoomMesh);

	auto car = ecs->CreateEntity();
	car->AddComponent<IdComponent>();
	car->AddComponent<NameComponent>("Car");
	car->AddComponent<PositionComponent>(glm::vec3(0, -130, 0));
	car->AddComponent<RotationComponent>(glm::vec3(130, 0, 0));
	car->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	car->AddComponent<UboModelComponent>();
	car->AddComponent<MeshComponent>(formulaMeshIndex);
	car->AddComponent<MaterialComponent>(formulaColorMaterialIndex);

	auto vikingRoomMeshPath = assetDatabase->meshesPaths.find("viking_room.obj");
	auto vikingsRoom = ecs->CreateEntity();
	vikingsRoom->AddComponent<IdComponent>();
	vikingsRoom->AddComponent<NameComponent>("Room");
	vikingsRoom->AddComponent<PositionComponent>(glm::vec3(0, 70, 7));
	vikingsRoom->AddComponent<RotationComponent>(glm::vec3(0, -70, 0));
	vikingsRoom->AddComponent<ScaleComponent>(glm::vec3(140, 140, 140));
	vikingsRoom->AddComponent<RotationVelocityComponent>(glm::vec3(0, 0, 50));
	vikingsRoom->AddComponent<UboModelComponent>();
	vikingsRoom->AddComponent<MeshComponent>(vikingsRoomMeshIndex);
	vikingsRoom->AddComponent<MaterialComponent>(vikingMaterialIndex);

	auto cameraEntity = ecs->CreateEntity();
	cameraEntity->AddComponent<IdComponent>();
	cameraEntity->AddComponent<NameComponent>("Camera");
	cameraEntity->AddComponent<PositionComponent>(glm::vec3(0, 100, 500));
	cameraEntity->AddComponent<UboModelComponent>();
	cameraEntity->AddComponent<UboViewProjectionComponent>();
	cameraEntity->AddComponent<CameraComponent>();

	auto diffuseLight = ecs->CreateEntity();
	diffuseLight->AddComponent<IdComponent>();
	diffuseLight->AddComponent<NameComponent>("Diffuse Light");
	diffuseLight->AddComponent<UboDiffuseLightComponent>();
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");
	ecs.reset();
}

void Level::InstantiatePrefab(const std::filesystem::path& path)
{
	auto serializer = CreateRef<EntitySerializer>(projectSettings);
	serializer->InstantiatePrefab(ecs, path);
}
