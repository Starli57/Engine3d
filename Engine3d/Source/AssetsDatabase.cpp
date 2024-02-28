#include "Pch.h"
#include "AssetsDatabase.h"
#include "Architecture/CustomAssert.h"

AssetsDatabase::AssetsDatabase()
{
	textures = std::unordered_map<std::string, Ref<Texture>>();
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

bool AssetsDatabase::HasTexture(const std::string& path)
{
	return textures.find(path) != textures.end();
}

Ref<Texture> AssetsDatabase::GetTexture(const std::string& path)
{
	auto it = textures.find(path);
	CAssert::Check(it != textures.end(), "Assets database dosn't have a texture with path: " + path);
	return it->second;
}

void AssetsDatabase::AddTexture(Ref<Texture> texture)
{
	if (!HasTexture(texture->path)) textures.insert({ texture->path, texture });
}

void AssetsDatabase::RemoveTexture(Ref<Texture> texture)
{
	auto it = textures.find(texture->path);
	CAssert::Check(it != textures.end(), "Assets database dosn't have a texture with path: " + texture->path);
	textures.erase(it->first);
}