#include "EngineCore/Pch.h"
#include "AssetsLoader.h"
#include "EngineCore/Assets/Meshes/CubeDefinition.h"
#include "EngineCore/Assets/Meshes/SphereDefinition.h"
#include "EngineCore/Utilities/YamlConverters.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <execution>

namespace EngineCore
{
	AssetsLoader::AssetsLoader(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, const Ref<AssetsDatabase>& assetsDatabase):
		graphicsApi(graphicsApi), projectSettings(projectSettings), assetsDatabase(assetsDatabase)
	{
		assetsDatabase->textureLoadStatuses.resize(assetsDatabase->texturesPaths.size());
		assetsDatabase->materialLoadStatuses.resize(assetsDatabase->materialsPaths.size());
		assetsDatabase->meshLoadStatuses.resize(assetsDatabase->meshesPaths.size());

		assetsDatabase->materials.resize(assetsDatabase->materialsPaths.size());

		//todo: replace out of constructor, because it should be initialized after AssetsLoaderVulkan constructor
		//SetupMesh(cubeDefinition);
		//CreateMesh(sphereDefiniton);
	}

	void AssetsLoader::Load()
	{
		LoadRequestedMaterials();
		LoadRequestedMeshes();
	}

	uint32_t AssetsLoader::GetTextureStr(const std::string& path) const
	{
		return GetTexture(std::filesystem::path(path));
	}

	uint32_t AssetsLoader::GetTexture(const std::filesystem::path& path) const
	{
		assetsDatabase->AddTextureLoadRequest(path);
		auto textureIter = assetsDatabase->texturesIndexByPath.find(path);
		CAssert::Check(textureIter != assetsDatabase->texturesIndexByPath.end(), "Texture file not found " + path.string());
		return textureIter->second;
	}

	uint32_t AssetsLoader::GetOrLoadTextureStr(const std::string& pathStr)
	{
		auto path = std::filesystem::path(pathStr);
		return GetOrLoadTexture(path);
	}

	uint32_t AssetsLoader::GetOrLoadTexture(const std::filesystem::path& path)
	{
		auto textureIter = assetsDatabase->texturesIndexByPath.find(path);
		if (!assetsDatabase->SetTextureLoadingStatus(path)) return textureIter->second;

		LoadTexture(path);
		return textureIter->second;
	}

	void AssetsLoader::DeserializeMeshMeta(const std::filesystem::path& path, MeshMeta& meshMeta) const
	{
		std::ifstream inFile(path.string(), std::ios::binary);
		if (!inFile)
		{
			spdlog::critical("Failed to open mesh file for reading by path={}", path.string());
			return;
		}

		size_t vertexCount;
		inFile.read(reinterpret_cast<char*>(&vertexCount), sizeof(size_t));
		meshMeta.vertices.resize(vertexCount);
		inFile.read(reinterpret_cast<char*>(meshMeta.vertices.data()), vertexCount * sizeof(Vertex));

		size_t indexCount;
		inFile.read(reinterpret_cast<char*>(&indexCount), sizeof(size_t));
		meshMeta.indices.resize(indexCount);
		inFile.read(reinterpret_cast<char*>(meshMeta.indices.data()), indexCount * sizeof(uint32_t));

		size_t materialPathSize;
		inFile.read(reinterpret_cast<char*>(&materialPathSize), sizeof(materialPathSize));
		meshMeta.materialPath.resize(materialPathSize);
		inFile.read(reinterpret_cast<char*>(&meshMeta.materialPath[0]), materialPathSize);

		size_t materialNameSize;
		inFile.read(reinterpret_cast<char*>(&materialNameSize), sizeof(materialNameSize));
		meshMeta.materialName.resize(materialNameSize);
		inFile.read(reinterpret_cast<char*>(&meshMeta.materialName[0]), materialNameSize);
		
		inFile.close();
	}

	void AssetsLoader::LoadMaterial(const std::filesystem::path& path, const uint32_t index)
	{
		std::vector<YAML::Node> data;

		try
		{
			data = YAML::LoadAllFromFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			spdlog::critical("Failed to load material by path={0} error={1}", path.string(), e.what());
			return;
		}

		if (data.size() > 1) spdlog::warn("Material file has more than 1 material at path={}", path.string());

		auto node = data[0];
		auto material = CreateRef<Material>(node["pipelineName"].as<std::string>());

		material->roughness = node["roughness"].as<float>();
		material->metallic = node["metallic"].as<float>();
		material->sheen = node["sheen"].as<float>();
		material->specularExponent = node["specularExponent"].as<float>();
		material->indexOfRefraction = node["indexOfRefraction"].as<float>();
		material->transparency = node["transparency"].as<float>();
		material->opaque = node["isOpaque"].as<bool>();
		
		material->ambientColor = node["ambientColor"].as<glm::vec3>();
		material->diffuseColor = node["diffuseColor"].as<glm::vec3>();
		material->specularColor = node["specularColor"].as<glm::vec3>();
		material->emissionColor = node["emissionColor"].as<glm::vec3>();

		auto diffuseTexturePath = node["diffuseTextureName"] ? node["diffuseTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto diffuseTextureIndex = GetOrLoadTextureStr(diffuseTexturePath);
		material->SetDiffuseTexture(diffuseTextureIndex);

		auto specularTexturePath = node["specularTextureName"] ? node["specularTextureName"].as<std::string>() : projectSettings->resourcesPath + "/black_box.png";
		auto specularTextureIndex = GetOrLoadTextureStr(specularTexturePath);
		material->SetSpecular(specularTextureIndex);

		auto normalTexturePath = node["bumpTextureName"] ? node["bumpTextureName"].as<std::string>() : projectSettings->resourcesPath + "/black_box.png";
		auto normalTextureIndex = GetOrLoadTextureStr(normalTexturePath);
		material->SetNormalMap(normalTextureIndex);

		auto alphaTexturePath = node["alphaTextureName"] ? node["alphaTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto alphaTextureIndex = GetOrLoadTextureStr(alphaTexturePath);
		material->SetAlphaMap(alphaTextureIndex);
	
		assetsDatabase->materials.at(index) = material;
		assetsDatabase->materialLoadStatuses.at(index) = 2;
	}

	void AssetsLoader::LoadAllMaterials()
	{
		std::for_each(std::execution::par, assetsDatabase->materialsIndexByPath.begin(), assetsDatabase->materialsIndexByPath.end(),
			[this](const auto& pair)
			{
				auto path = pair.first;
				auto index = pair.second;
				LoadMaterial(path, index);
			});
	}

	void AssetsLoader::LoadRequestedMeshes()
	{
		std::vector<uint32_t> loadMeshes;
		assetsDatabase->meshLoadStatusMutex.lock();
		{
			loadMeshes.resize(assetsDatabase->meshLoadRequests.size());
		
			for (uint32_t i = 0; i < loadMeshes.size(); i++)
			{
				loadMeshes[i] = assetsDatabase->meshLoadRequests.front();
				assetsDatabase->meshLoadRequests.pop();
			}
		}
		assetsDatabase->meshLoadStatusMutex.unlock();
		if (loadMeshes.empty()) return;
	
		std::for_each(std::execution::par, loadMeshes.begin(), loadMeshes.end(), [this](const auto& meshIndex)
			{
				auto path = assetsDatabase->meshesPaths[meshIndex];
				LoadMesh(path);
			});
	}

	void AssetsLoader::LoadRequestedMaterials()
	{
		std::vector<uint32_t> loadMaterials;
		assetsDatabase->materialLoadStatusMutex.lock();
		{
			loadMaterials.resize(assetsDatabase->materialLoadRequests.size());
	
			for (uint32_t i = 0; i < loadMaterials.size(); i++)
			{
				loadMaterials[i] = assetsDatabase->materialLoadRequests.front();
				assetsDatabase->materialLoadRequests.pop();
			}
		}
		assetsDatabase->materialLoadStatusMutex.unlock();
		if (loadMaterials.empty()) return;
	
		std::for_each(std::execution::par, loadMaterials.begin(), loadMaterials.end(), [this](const auto& materialIndex)
			{
				auto path = assetsDatabase->materialsPaths[materialIndex];
				LoadMaterial(path, materialIndex);
			});
	}
}