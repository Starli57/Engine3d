#include "EngineCore/Pch.h"
#include "EngineCore/Core/AssetsDatabase.h"
#include "EngineCore/Utilities/IOUtility.h"

AssetsDatabase::AssetsDatabase(const Ref<ProjectSettings>& projectSettings)
	: projectSettings(projectSettings)
{
	FillMeshesPaths();
	FillMaterialsPaths();
	FillPrefabsPaths();
	FillTexturesPaths();
	FillWorldsPaths();
}

void AssetsDatabase::FillMeshesPaths()
{
	spdlog::info("FillMeshesPaths");
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".mesh");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, meshesPaths, meshesIndexByPath);
}

void AssetsDatabase::FillMaterialsPaths()
{
	spdlog::info("FillMaterialsPaths");
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".material");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, materialsPaths, materialsIndexByPath);
}

void AssetsDatabase::FillPrefabsPaths()
{
	spdlog::info("FillPrefabsPaths");
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".yaml");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->prefabsPath, relevantExtensions, prefabsPaths, prefabsIndexByPath);
}

void AssetsDatabase::FillTexturesPaths()
{
	spdlog::info("FillTexturesPaths");
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(2);
	relevantExtensions.emplace_back(".png");
	relevantExtensions.emplace_back(".jpg");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, texturesPaths, texturesIndexByPath);
}

void AssetsDatabase::FillWorldsPaths()
{
	spdlog::info("FillWorldsPaths");
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.emplace_back(".world");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->worldsPath, relevantExtensions, worldsPaths, worldsIndexByPath);
}

