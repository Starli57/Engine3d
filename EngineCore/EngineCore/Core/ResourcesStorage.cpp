#include "EngineCore/Pch.h"
#include "EngineCore/Core/ResourcesStorage.h"

#include "EngineCore/Assets/Meshes/CubeDefinition.h"
#include "EngineCore/Utilities/IOUtility.h"

namespace EngineCore
{
	ResourcesStorage::ResourcesStorage(const Ref<ProjectSettings>& projectSettings)
		: projectSettings(projectSettings)
	{
		FillAssetsPaths();
		FillPrefabsPaths();
		FillWorldsPaths();
		FillCustomMeshes();
	}

	bool ResourcesStorage::SetMeshLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = meshesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(meshLoadStatusMutex);
	
		if (meshLoadStatuses[iter->second] != 0) return false;
		meshLoadStatuses[iter->second] = 1;

		return true;
	}

	bool ResourcesStorage::SetMaterialLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = materialsIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(materialLoadStatusMutex);
	
		if (materialLoadStatuses[iter->second] != 0) return false;
		materialLoadStatuses[iter->second] = 1;
	
		return true;
	}

	bool ResourcesStorage::SetTextureLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = texturesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(textureLoadStatusMutex);
	
		if (textureLoadStatuses[iter->second] != 0) return false;
		textureLoadStatuses[iter->second] = 1;
	
		return true;
	}

	void ResourcesStorage::AddMeshLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = meshesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(meshLoadStatusMutex);
	
		if (meshLoadStatuses[iter->second] != 0) return;
		meshLoadStatuses[iter->second] = 1;
	
		meshLoadRequests.emplace(iter->second);
	}

	void ResourcesStorage::AddMaterialLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = materialsIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(materialLoadStatusMutex);
	
		if (materialLoadStatuses[iter->second] != 0) return;
		materialLoadStatuses[iter->second] = 1;
	
		materialLoadRequests.emplace(iter->second);
	}

	void ResourcesStorage::AddTextureLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = texturesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(textureLoadStatusMutex);
	
		if (textureLoadStatuses[iter->second] != 0) return;
		textureLoadStatuses[iter->second] = 1;
	
		textureLoadRequests.emplace(iter->second);
	}

	void ResourcesStorage::FillAssetsPaths()
	{
		spdlog::info("Fill Assets Paths");

		std::unordered_map<std::filesystem::path, uint32_t> allAssetsPaths;
		IOUtility().FindResourcesFiles(projectSettings->resourcesPath, allAssetsPaths);

		for (auto& pathPair : allAssetsPaths)
		{
			auto path = pathPair.first;

			if (path.has_extension() == false) continue;

			if (path.extension().string() == ".mesh")
			{
				meshesIndexByPath.emplace(path, meshesPaths.size());
				meshesPaths.emplace_back(path);
			}

			if (path.extension().string() == ".material")
			{
				materialsIndexByPath.emplace(path, materialsPaths.size());
				materialsPaths.emplace_back(path);
			}

			if (path.extension().string() == ".yaml")
			{
				prefabsIndexByPath.emplace(path, prefabsPaths.size());
				prefabsPaths.emplace_back(path);
			}

			if (path.extension().string() == ".png" || path.extension().string() == ".jpg")
			{
				texturesIndexByPath.emplace(path, texturesPaths.size());
				texturesPaths.emplace_back(path);
			}

			if (path.extension().string() == ".world")
			{
				worldsIndexByPath.emplace(path, worldsPaths.size());
				worldsPaths.emplace_back(path);
			}

			if (path.extension().string() == ".combinedmesh")
			{
				combinedMeshIndexByPath.emplace(path, combinedMeshPaths.size());
				combinedMeshPaths.emplace_back(path);
			}
		}
	}

	void ResourcesStorage::FillMeshesPaths()
	{
		spdlog::info("FillMeshesPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".mesh");

		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, meshesPaths, meshesIndexByPath);
		meshLoadStatuses.resize(meshesPaths.size(), 0);
	}

	void ResourcesStorage::FillMaterialsPaths()
	{
		spdlog::info("FillMaterialsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".material");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, materialsPaths, materialsIndexByPath);
		meshLoadStatuses.resize(materialsPaths.size(), 0);
	}

	void ResourcesStorage::FillPrefabsPaths()
	{
		spdlog::info("FillPrefabsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".yaml");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->prefabsPath, relevantExtensions, prefabsPaths, prefabsIndexByPath);
	}

	void ResourcesStorage::FillTexturesPaths()
	{
		spdlog::info("FillTexturesPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(2);
		relevantExtensions.emplace_back(".png");
		relevantExtensions.emplace_back(".jpg");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, texturesPaths, texturesIndexByPath);
		meshLoadStatuses.resize(texturesPaths.size(), 0);
	}

	void ResourcesStorage::FillWorldsPaths()
	{
		spdlog::info("FillWorldsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.emplace_back(".world");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->worldsPath, relevantExtensions, worldsPaths, worldsIndexByPath);
	}

	void ResourcesStorage::FillCustomMeshes()
	{
		auto cubeDefinition = CubeDefinition();
		customMeshes.emplace(meshesPaths.size() + 0, cubeDefinition);
		//SetupMesh(cubeDefinition);

		//auto sphereDefiniton = SphereDefinition();
		//customMeshes.emplace(meshesPaths.size() + 1, sphereDefiniton);
	}
}