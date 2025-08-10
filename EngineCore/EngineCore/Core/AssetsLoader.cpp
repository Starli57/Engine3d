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
	AssetsLoader::AssetsLoader(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, const Ref<ResourcesStorage>& assetsDatabase):
		graphicsApi(graphicsApi), projectSettings(projectSettings), assetsDatabase(assetsDatabase)
	{
		assetsDatabase->textureLoadStatuses.resize(assetsDatabase->texturesPaths.size());
		assetsDatabase->materialLoadStatuses.resize(assetsDatabase->materialsPaths.size());
		assetsDatabase->meshLoadStatuses.resize(assetsDatabase->meshesPaths.size());

		assetsDatabase->materials.resize(assetsDatabase->materialsPaths.size());

		//todo: replace out of constructor, because it should be initialized after AssetsLoaderVulkan constructor
		//SetupMeshBuffers(cubeDefinition);
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

	void AssetsLoader::LoadAndDeserializeMesh(const std::filesystem::path& path, MeshMeta& meshMeta) const
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

	void AssetsLoader::LoadAndDeserializeMaterial(const std::filesystem::path& path, const Ref<PbrMaterial>& material)
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

		if (data.size() > 1) spdlog::warn("PbrMaterial file has more than 1 material at path={}", path.string());

		auto node = data[0];
		material->pipelineId = node["pipelineName"].as<std::string>();
		material->opaque = node["isOpaque"].as<bool>();

		material->roughness = node["roughness"].as<float>();
		material->metallic = node["metallic"].as<float>();
		material->baseColor = node["baseColor"].as<glm::vec4>();
		
		auto baseColorTexturePath = node["baseColorTextureName"] ? node["baseColorTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto diffuseTextureIndex = GetOrLoadTextureStr(baseColorTexturePath);
		material->baseTexture = diffuseTextureIndex;

		auto normalTexturePath = node["normalsTextureName"] ? node["normalsTextureName"].as<std::string>() : projectSettings->resourcesPath + "/black_box.png";
		auto normalTextureIndex = GetOrLoadTextureStr(normalTexturePath);
		material->normalsTexture = normalTextureIndex;

		auto metallicRoughnessPath = node["metallicRoughnessTextureName"] ? node["metallicRoughnessTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto metallicRoughnessTextureIndex = GetOrLoadTextureStr(metallicRoughnessPath);
		material->metallicRoughnessTexture = metallicRoughnessTextureIndex;
		
		auto occlusionTexturePath = node["occlusionTextureName"] ? node["occlusionTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto occlusionTextureIndex = GetOrLoadTextureStr(occlusionTexturePath);
		material->occlusionTexture = occlusionTextureIndex;
		
		auto emissiveTexturePath = node["emissiveTextureName"] ? node["emissiveTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto emissiveTextureIndex = GetOrLoadTextureStr(emissiveTexturePath);
		material->emissiveTexture = emissiveTextureIndex;
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
				LoadMaterial(path);
			});
	}
}