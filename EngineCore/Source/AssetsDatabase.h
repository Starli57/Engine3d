#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "Entities/Material.h"
#include "Entities/Texture.h"

#include "EngineShared/Ref.h"
#include "EngineShared/UniqueId.h"
#include "EngineShared/CustomAssert.h"
#include "EngineShared/ProjectSettings.h"

class AssetsDatabase
{
public:
	std::unordered_map<std::string, std::filesystem::path> meshesPaths;
	std::unordered_map<std::string, std::filesystem::path> texturesPaths;

	AssetsDatabase(Ref<ProjectSettigns> projectSettings);
	~AssetsDatabase();


	int32_t TextureIndex(const std::filesystem::path& texturePath);
	int32_t TextureIndex(UniqueId uniqueId);
	bool HasTexture(const std::filesystem::path& texturePath);

	Ref<Texture> GetTexture(const int32_t index);
	Ref<Texture> GetTexture(const std::filesystem::path& texturePath);
	
	int32_t AddTexture(Ref<Texture> texture);
	void RemoveTexture(const std::filesystem::path& texturePath);


	int32_t MaterialIndex(const UniqueId uniqueId);

	Ref<Material> GetMaterial(const int32_t index);

	int32_t AddMaterial(Ref<Material> material);
	void RemoveMaterial(Ref<Material> material);

private:
	Ref<ProjectSettigns> projectSettings;

	std::vector<Ref<Material>> materials;
	std::vector<Ref<Texture>> textures;

	void FillMeshesPaths();
	void FillTexturesPaths();
};