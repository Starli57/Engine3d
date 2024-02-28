#include "Pch.h"
#include "AssetsDatabaseVulkan.h"
#include "Architecture/CustomAssert.h"

namespace AVulkan
{
	AssetsDatabaseVulkan::AssetsDatabaseVulkan()
	{
		textures = std::unordered_map<std::string, Ref<Texture>>();
	}
	
	AssetsDatabaseVulkan::~AssetsDatabaseVulkan()
	{
		//todo: check if vk objects are disposed correctly
		for (auto& pair : textures) {
			Ref<Texture>& textureRef = pair.second;
			textureRef.reset();
		}

		textures.clear();
	}

	bool AssetsDatabaseVulkan::HasTexture(const std::string& path)
	{
		return textures.find(path) != textures.end();
	}

	Ref<Texture> AssetsDatabaseVulkan::GetTexture(const std::string& path)
	{
		auto it = textures.find(path);
		CAssert::Check(it != textures.end(), "Assets database dosn't have a texture with path: " + path);
		return it->second;
	}

	void AssetsDatabaseVulkan::AddTexture(Ref<Texture> texture)
	{
		if (!HasTexture(texture->path)) textures.insert({ texture->path, texture });
	}

	void AssetsDatabaseVulkan::RemoveTexture(Ref<Texture> texture)
	{
		auto it = textures.find(texture->path);
		CAssert::Check(it != textures.end(), "Assets database dosn't have a texture with path: " + texture->path);
		textures.erase(it->first);
	}
}