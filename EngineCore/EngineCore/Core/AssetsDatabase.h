#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Utilities/UniqueId.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rendering/Vertex.h"

class AssetsDatabase
{
public:
	std::vector<std::filesystem::path> meshesPaths;
	std::vector<std::filesystem::path> materialsPaths;
	std::vector<std::filesystem::path> prefabsPaths;
	std::vector<std::filesystem::path> texturesPaths;
	std::vector<std::filesystem::path> worldsPaths;

	std::unordered_map<std::filesystem::path, uint32_t> meshesIndexByPath;
	std::unordered_map<std::filesystem::path, uint32_t> materialsIndexByPath;
	std::unordered_map<std::filesystem::path, uint32_t> prefabsIndexByPath;
	std::unordered_map<std::filesystem::path, uint32_t> texturesIndexByPath;
	std::unordered_map<std::filesystem::path, uint32_t> worldsIndexByPath;

	std::vector<Ref<Material>> materials;
	std::vector<uint32_t> meshMaterialBinding;
	
	AssetsDatabase(const Ref<ProjectSettings>& projectSettings);
	virtual ~AssetsDatabase() = default;

	size_t MeshesCount() const { return meshesPaths.size(); }
	size_t MaterialsCount() const { return materials.size(); }
	size_t TexturesCount() const { return texturesPaths.size(); }

private:
	Ref<ProjectSettings> projectSettings;

	void FillMeshesPaths();
	void FillMaterialsPaths();
	void FillPrefabsPaths();
	void FillTexturesPaths();
	void FillWorldsPaths();
};