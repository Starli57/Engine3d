#include "Pch.h"
#include "AssetsDatabase.h"
#include "EngineShared/IOUtility.h"

//todo: calculate texture links count to be able to dispose if not used

AssetsDatabase::AssetsDatabase(Ref<ProjectSettigns> projectSettings)
{
	this->projectSettings = projectSettings;

	textures = std::unordered_map<std::filesystem::path, Ref<Texture>>();

	FillMeshesPaths();
	FillTexturesPaths();
}
	
AssetsDatabase::~AssetsDatabase()
{
	//todo: check if vk objects are disposed correctly
	for (auto& pair : textures) {
		Ref<Texture>& textureRef = pair.second;
		textureRef.reset();
	}

	textures.clear();
}

bool AssetsDatabase::HasTexture(const std::filesystem::path& texturePath)
{
	return textures.find(texturePath) != textures.end();
}

Ref<Texture> AssetsDatabase::GetTexture(const std::filesystem::path& texturePath)
{
	auto it = textures.find(texturePath);
	CAssert::Check(it != textures.end(), "Assets database doesn't have a texture with path: " + texturePath.string());
	return it->second;
}

void AssetsDatabase::AddTexture(Ref<Texture> texture)
{
	if (!HasTexture(texture->textureFilePath)) textures.insert({ texture->textureFilePath, texture });
}

void AssetsDatabase::RemoveTexture(const std::filesystem::path& texturePath)
{
	auto it = textures.find(texturePath);
	CAssert::Check(it != textures.end(), "Assets database doesn't have a texture with path: " + texturePath.string());
	textures.erase(it->first);
}

void AssetsDatabase::FillMeshesPaths()
{
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".obj");
	IOUtility().FindResourcesFiles(projectSettings->resourcesPath, relevantExtensions, meshesPaths);
}

void AssetsDatabase::FillTexturesPaths()
{
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".png");
	IOUtility().FindResourcesFiles(projectSettings->resourcesPath, relevantExtensions, texturesPaths);
}
