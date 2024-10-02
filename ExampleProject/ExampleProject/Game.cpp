#include "Game.h"

Game::Game(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine>(projectSettings);
	editor = CreateRef<Editor>(projectSettings, engine);

	assetDatabase = engine->GetAssetsDatabase();
	resourcesManager = engine->GetResourcesManager();
	graphicsApi = engine->GetGraphicsApi();
	ecs = engine->GetEcs();

	LoadLevel();
}

Game::~Game()
{
	editor.reset();
	engine.reset();
}

void Game::Run() 
{
	engine->Run();
}

void Game::LoadLevel()
{
	spdlog::info("Load level");

	//textures
	auto vikignTextureIndex = resourcesManager->LoadTextureByName("viking_room.png");
	auto formulaTextureIndex = resourcesManager->LoadTextureByName("formula1_DefaultMaterial_Diffuse.png");
	auto emptyTextureIndex = resourcesManager->LoadTextureByName("empty.png");

	//materials
	std::string albedoOpaquePipelineId = "albedoOpaque";
	std::string edgesPipelineId = "edges";
	auto vikingMaterialIndex = resourcesManager->CreateMaterial(albedoOpaquePipelineId, vikignTextureIndex);
	auto formulaMaterialIndex = resourcesManager->CreateMaterial(albedoOpaquePipelineId, formulaTextureIndex);
	auto colorMaterialIndex = resourcesManager->CreateMaterial(albedoOpaquePipelineId, emptyTextureIndex);
	auto edgesMaterialIndex = resourcesManager->CreateMaterial(edgesPipelineId, emptyTextureIndex);

	//meshes
	auto formulaMeshIndex = resourcesManager->LoadMeshByName("Formula_1_mesh.obj");
	auto vikingsRoomMeshIndex = resourcesManager->LoadMeshByName("viking_room.obj");
	auto porsheMeshIndex = resourcesManager->LoadMeshByName("scene.gltf");

	//auto car = ecs->CreateEntity();
	//car->AddComponent<IdComponent>();
	//car->AddComponent<NameComponent>("Car");
	//car->AddComponent<PositionComponent>(glm::vec3(0, -130, 0));
	//car->AddComponent<RotationComponent>(glm::vec3(130, 0, 0));
	//car->AddComponent<ScaleComponent>(glm::vec3(1, 1, 1));
	//car->AddComponent<UboModelComponent>();
	//car->AddComponent<MeshComponent>(formulaMeshIndex);
	//car->AddComponent<MaterialComponent>(colorMaterialIndex);

	//auto vikingsRoom = ecs->CreateEntity();
	//vikingsRoom->AddComponent<IdComponent>();
	//vikingsRoom->AddComponent<NameComponent>("Room");
	//vikingsRoom->AddComponent<PositionComponent>(glm::vec3(0, 70, 7));
	//vikingsRoom->AddComponent<RotationComponent>(glm::vec3(0, -70, 0));
	//vikingsRoom->AddComponent<ScaleComponent>(glm::vec3(140, 140, 140));
	//vikingsRoom->AddComponent<RotationVelocityComponent>(glm::vec3(0, 0, 50));
	//vikingsRoom->AddComponent<UboModelComponent>();
	//vikingsRoom->AddComponent<MeshComponent>(vikingsRoomMeshIndex);
	//vikingsRoom->AddComponent<MaterialComponent>(vikingMaterialIndex);

	//auto cameraEntity = ecs->CreateEntity();
	//cameraEntity->AddComponent<IdComponent>();
	//cameraEntity->AddComponent<NameComponent>("Camera");
	//cameraEntity->AddComponent<PositionComponent>(glm::vec3(0, 0, 500));
	//cameraEntity->AddComponent<RotationComponent>(glm::vec3(-90, 0, 0));
	//cameraEntity->AddComponent<UboModelComponent>();
	//cameraEntity->AddComponent<UboViewProjectionComponent>();
	//cameraEntity->AddComponent<CameraComponent>();
	//cameraEntity->AddComponent<CameraFreeComponent>();

	//auto diffuseLight = ecs->CreateEntity();
	//diffuseLight->AddComponent<IdComponent>();
	//diffuseLight->AddComponent<NameComponent>("Diffuse Light");
	//diffuseLight->AddComponent<UboDiffuseLightComponent>();
}
