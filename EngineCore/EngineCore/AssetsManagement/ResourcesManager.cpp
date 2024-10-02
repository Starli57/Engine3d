#include "EngineCore/Pch.h"
#include "ResourcesManager.h"
#include "EngineCore/Assets/Meshes/CubeDefinition.h"
#include "EngineCore/Assets/Meshes/SphereDefinition.h"

ResourcesManager::ResourcesManager(IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase):
	graphicsApi(graphicsApi), assetsDatabase(assetsDatabase)
{
	auto cubeDefinition = CubeDefinition();
	CreateMesh(cubeDefinition);

	auto sphereDefiniton = SphereDefinition();
	CreateMesh(sphereDefiniton);
}

ResourcesManager::~ResourcesManager()
{
}

int32_t ResourcesManager::LoadTextureByName(const std::string& fileName)
{
	auto texturePath = assetsDatabase->texturesPaths.find(fileName);
	return LoadTextureByPath(texturePath->second);
}

int32_t ResourcesManager::LoadTextureByPath(const std::filesystem::path& path)
{
	auto texture = graphicsApi->CreateTexture(path);
	auto textureIndex = assetsDatabase->AddTexture(texture);
	return textureIndex;
}

int32_t ResourcesManager::LoadMeshByName(const std::string& fileName)
{
	auto meshPath = assetsDatabase->meshesPaths.find(fileName);
	return LoadMeshByPath(meshPath->second);
}

int32_t ResourcesManager::LoadMeshByPath(const std::filesystem::path& path)
{
	auto mesh = graphicsApi->LoadMesh(path);
	auto meshIndex = assetsDatabase->AddMesh(mesh);
	return meshIndex;
}

int32_t ResourcesManager::CreateMesh(MeshDefinition& meshDefinition)
{
	return CreateMesh(meshDefinition.vertices, meshDefinition.indices);
}

int32_t ResourcesManager::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
{
	auto mesh = graphicsApi->CreateMesh(vertices, indices);
	auto meshIndex = assetsDatabase->AddMesh(mesh);
	return meshIndex;
}

int32_t ResourcesManager::CreateMaterial(const std::string& pipelineId)
{
	auto material = graphicsApi->CreateMaterial(pipelineId);
	auto vikingMaterialIndex = assetsDatabase->AddMaterial(material);
	return vikingMaterialIndex;
}

int32_t ResourcesManager::CreateMaterial(const std::string& pipelineId, int32_t albedoTextureIndex)
{
	auto materialIndex = CreateMaterial(pipelineId);
	assetsDatabase->GetMaterial(materialIndex)->SetAlbedoTexture(albedoTextureIndex);
	return materialIndex;
}
