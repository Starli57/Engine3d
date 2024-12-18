#include "Game.h"

Game::Game(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine>(projectSettings);
	editor = CreateRef<Editor>(projectSettings, engine);

	assetDatabase = engine->GetAssetsDatabase();
	resourcesManager = engine->GetResourcesManager();
	graphicsApi = engine->GetGraphicsApi();
	ecs = engine->GetEcs();

	LoadResources();
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

void Game::LoadResources()
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

}
