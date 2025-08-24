#include "EngineCore/Pch.h"
#include "ResourcesStorageVulkan.h"

#include "EngineCore/Utilities/IOUtility.h"

namespace EngineCore
{
	ResourcesStorageVulkan::ResourcesStorageVulkan(const Ref<ProjectSettings>& projectSettings)
		: projectSettings(projectSettings)
	{
		FillAssetsPaths();
		FillPrefabsPaths();
		FillWorldsPaths();
		FillCustomMeshes();
	}

	bool ResourcesStorageVulkan::SetMeshLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = meshesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(meshLoadStatusMutex);
	
		if (meshLoadStatuses[iter->second] != 0) return false;
		meshLoadStatuses[iter->second] = 1;

		return true;
	}

	bool ResourcesStorageVulkan::SetMaterialLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = materialsIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(materialLoadStatusMutex);
	
		if (materialLoadStatuses[iter->second] != 0) return false;
		materialLoadStatuses[iter->second] = 1;
	
		return true;
	}

	bool ResourcesStorageVulkan::SetTextureLoadingStatus(const std::filesystem::path& path)
	{
		const auto iter = texturesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(textureLoadStatusMutex);
	
		if (textureLoadStatuses[iter->second] != 0) return false;
		textureLoadStatuses[iter->second] = 1;
	
		return true;
	}

	void ResourcesStorageVulkan::AddMeshLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = meshesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(meshLoadStatusMutex);
	
		if (meshLoadStatuses[iter->second] != 0) return;
		meshLoadStatuses[iter->second] = 1;
	
		meshLoadRequests.emplace(iter->second);
	}

	void ResourcesStorageVulkan::AddMaterialLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = materialsIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(materialLoadStatusMutex);
	
		if (materialLoadStatuses[iter->second] != 0) return;
		materialLoadStatuses[iter->second] = 1;
	
		materialLoadRequests.emplace(iter->second);
	}

	void ResourcesStorageVulkan::AddTextureLoadRequest(const std::filesystem::path& path)
	{
		const auto iter = texturesIndexByPath.find(path);
		std::lock_guard<std::mutex> lock(textureLoadStatusMutex);
	
		if (textureLoadStatuses[iter->second] != 0) return;
		textureLoadStatuses[iter->second] = 1;
	
		textureLoadRequests.emplace(iter->second);
	}

	void ResourcesStorageVulkan::FillAssetsPaths()
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

	void ResourcesStorageVulkan::FillMeshesPaths()
	{
		spdlog::info("FillMeshesPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".mesh");

		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, meshesPaths, meshesIndexByPath);
		meshLoadStatuses.resize(meshesPaths.size(), 0);
	}

	void ResourcesStorageVulkan::FillMaterialsPaths()
	{
		spdlog::info("FillMaterialsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".material");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, materialsPaths, materialsIndexByPath);
		meshLoadStatuses.resize(materialsPaths.size(), 0);
	}

	void ResourcesStorageVulkan::FillPrefabsPaths()
	{
		spdlog::info("FillPrefabsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.push_back(".yaml");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->prefabsPath, relevantExtensions, prefabsPaths, prefabsIndexByPath);
	}

	void ResourcesStorageVulkan::FillTexturesPaths()
	{
		spdlog::info("FillTexturesPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(4);
		relevantExtensions.emplace_back(".png");
		relevantExtensions.emplace_back(".jpg");
		relevantExtensions.emplace_back(".JPG");
		relevantExtensions.emplace_back(".jpeg");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, texturesPaths, texturesIndexByPath);
		meshLoadStatuses.resize(texturesPaths.size(), 0);
	}

	void ResourcesStorageVulkan::FillWorldsPaths()
	{
		spdlog::info("FillWorldsPaths");
		auto relevantExtensions = std::vector<std::string>();
		relevantExtensions.reserve(1);
		relevantExtensions.emplace_back(".world");
	
		IOUtility().FindAndEmplaceResourcesFiles(projectSettings->worldsPath, relevantExtensions, worldsPaths, worldsIndexByPath);
	}

	void ResourcesStorageVulkan::FillCustomMeshes()
	{

	}
}
