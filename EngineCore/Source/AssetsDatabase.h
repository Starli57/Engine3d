#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "EngineShared/Ref.h"
#include "EngineShared/CustomAssert.h"
#include "EngineShared/ProjectSettings.h"
#include "Entities/Texture.h"

class AssetsDatabase
{
public:
	std::unordered_map<std::string, std::filesystem::path> meshesPaths;
	std::unordered_map<std::string, std::filesystem::path> texturesPaths;

	AssetsDatabase(Ref<ProjectSettigns> projectSettings);
	~AssetsDatabase();

	int32_t TextureIndex(const std::filesystem::path& texturePath);
	bool HasTexture(const std::filesystem::path& texturePath);

	Ref<Texture> GetTexture(const int index);
	Ref<Texture> GetTexture(const std::filesystem::path& texturePath);
	
	int32_t AddTexture(Ref<Texture> texture);
	void RemoveTexture(const std::filesystem::path& texturePath);

private:
	Ref<ProjectSettigns> projectSettings;
	std::vector<Ref<Texture>> textures;

	void FillMeshesPaths();
	void FillTexturesPaths();
};