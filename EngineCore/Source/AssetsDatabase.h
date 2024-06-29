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

	bool HasTexture(const std::filesystem::path& texturePath);
	Ref<Texture> GetTexture(const std::filesystem::path& texturePath);
	void AddTexture(Ref<Texture> texture);
	void RemoveTexture(const std::filesystem::path& texturePath);

private:
	Ref<ProjectSettigns> projectSettings;
	std::unordered_map<std::filesystem::path, Ref<Texture>> textures;

	void FillMeshesPaths();
	void FillTexturesPaths();
};