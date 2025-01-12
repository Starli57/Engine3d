#include "EngineCore/Pch.h"
#include "AssetsLoader.h"
#include "EngineCore/Assets/Meshes/CubeDefinition.h"
#include "EngineCore/Assets/Meshes/SphereDefinition.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <execution>


AssetsLoader::AssetsLoader(Ref<ProjectSettings> projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase):
	projectSettings(projectSettings), graphicsApi(graphicsApi), assetsDatabase(assetsDatabase)
{
}

void AssetsLoader::Load()
{
	LoadAllTextures();
	LoadAllMaterials();

	std::vector<MeshMeta> meshesMeta;
	PrepareAllMeshesMeta(meshesMeta);
	LoadAllMeshes(meshesMeta);

	/*auto cubeDefinition = CubeDefinition();
	CreateMesh(cubeDefinition);

	auto sphereDefiniton = SphereDefinition();
	CreateMesh(sphereDefiniton);*/

	//	for (const auto& meshPath : assetsDatabase->meshesPathByName) LoadMeshByPath(meshPath.second);

	//todo: make threads possible
	//std::for_each(std::execution::par, assetsDatabase->materialsPaths.begin(), assetsDatabase->materialsPaths.end(), [this](const std::pair<std::string, std::filesystem::path>& path)
	//{
	//	LoadMaterialByName(path.first);
	//});

	//std::for_each(std::execution::par, assetsDatabase->meshesPaths.begin(), assetsDatabase->meshesPaths.end(), [this](const std::pair<std::string, std::filesystem::path>& path)
	//{
	//	LoadMeshByPath(path.second);
	//});
}

uint32_t AssetsLoader::LoadTextureStr(const std::string& path)
{
	return LoadTexture(std::filesystem::path(path));
}

uint32_t AssetsLoader::LoadTexture(const std::filesystem::path& path)
{
	auto textureIndex = assetsDatabase->texturesIndexByPath.find(path);
	CAssert::Check(textureIndex != assetsDatabase->texturesIndexByPath.end(), "Texture file not found " + path.string());
	return textureIndex->second;
}

void AssetsLoader::PrepareAllMeshesMeta(std::vector<MeshMeta>& meshes)
{
	auto meshesCount = assetsDatabase->meshesIndexByPath.size();
	meshes.resize(meshesCount);

	std::for_each(std::execution::par, assetsDatabase->meshesIndexByPath.begin(), assetsDatabase->meshesIndexByPath.end(), [this, &meshes](const auto& pair)
	{
		auto path = pair.first;
		auto index = pair.second;

		std::ifstream inFile(path.string(), std::ios::binary);
		if (!inFile)
		{
			spdlog::critical("Failed to open mesh file for reading by path={}", path.string());
			return;
		}

		MeshMeta& meshMeta = meshes.at(index);

		size_t vertexCount;
		inFile.read(reinterpret_cast<char*>(&vertexCount), sizeof(size_t));
		meshMeta.vertices.resize(vertexCount);
		inFile.read(reinterpret_cast<char*>(meshMeta.vertices.data()), vertexCount * sizeof(Vertex));

		size_t indexCount;
		inFile.read(reinterpret_cast<char*>(&indexCount), sizeof(size_t));
		meshMeta.indices.resize(indexCount);
		inFile.read(reinterpret_cast<char*>(meshMeta.indices.data()), indexCount * sizeof(uint32_t));

		size_t materiaPathSize;
		inFile.read(reinterpret_cast<char*>(&materiaPathSize), sizeof(materiaPathSize));
		meshMeta.materialPath.resize(materiaPathSize);
		inFile.read(reinterpret_cast<char*>(&meshMeta.materialPath[0]), materiaPathSize);

		size_t materialNameSize;
		inFile.read(reinterpret_cast<char*>(&materialNameSize), sizeof(materialNameSize));
		meshMeta.materialName.resize(materialNameSize);
		inFile.read(reinterpret_cast<char*>(&meshMeta.materialName[0]), materialNameSize);
		
		inFile.close();
	});
}

void AssetsLoader::LoadAllMeshes(std::vector<MeshMeta>& meshes)
{
	throw std::runtime_error("LoadAllMeshes method is not implemented in base AssetsLoader");
}

void AssetsLoader::UnLoadAllMeshes()
{
	throw std::runtime_error("UnLoadAllMeshes method is not implemented in base AssetsLoader");
}

void AssetsLoader::LoadAllTextures()
{
	throw std::runtime_error("LoadAllTextures method is not implemented in base AssetsLoader");
}

void AssetsLoader::UnLoadAllTextures()
{
	throw std::runtime_error("UnLoadAllTextures method is not implemented in base AssetsLoader");
}

void AssetsLoader::LoadAllMaterials()
{
	assetsDatabase->materials.resize(assetsDatabase->materialsIndexByPath.size());
	
	std::for_each(std::execution::par, assetsDatabase->materialsIndexByPath.begin(), assetsDatabase->materialsIndexByPath.end(),
		[this](const auto& pair)
	{
		auto path = pair.first;
		auto index = pair.second;
		std::vector<YAML::Node> data;

		try
		{
			data = YAML::LoadAllFromFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			spdlog::critical("Failed to load material by path={0} error={1}", path.string(), e.what());
			return -1;
		}
	
		if (data.size() > 1) spdlog::warn("Material file has more than 1 material at path={}", path.string());

		auto node = data[0];
		auto material = CreateRef<Material>(node["pipelineName"].as<std::string>());

		if (node["diffuseTextureName"]) material->SetDiffuseTexture(LoadTextureStr(node["diffuseTextureName"].as<std::string>()));
		else material->SetDiffuseTexture(LoadTextureStr(projectSettings->resourcesPath + "/white_box.png"));
			
		if (node["specularTextureName"]) material->SetSpecular(LoadTextureStr(node["specularTextureName"].as<std::string>()));
		else material->SetSpecular(LoadTextureStr(projectSettings->resourcesPath + "/black_box.png"));
			
		if (node["bumpTextureName"]) material->SetNormalMap(LoadTextureStr(node["bumpTextureName"].as<std::string>()));
		else material->SetNormalMap(LoadTextureStr(projectSettings->resourcesPath + "/black_box.png"));
			
		if (node["alphaTextureName"]) material->SetAlphaMap(LoadTextureStr(node["alphaTextureName"].as<std::string>()));
		else material->SetAlphaMap(LoadTextureStr(projectSettings->resourcesPath + "/white_box.png"));
			
		assetsDatabase->materials.at(index) = material;
	});
}