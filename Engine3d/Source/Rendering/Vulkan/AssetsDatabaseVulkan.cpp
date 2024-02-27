#include "Pch.h"
#include "AssetsDatabaseVulkan.h"

namespace AVulkan
{
	AssetsDatabaseVulkan::AssetsDatabaseVulkan()
	{
		textures = CreateUniqueRef<std::vector<VkImage>>();
		texturesMemory = CreateUniqueRef<std::vector<VkDeviceMemory>>();
	}
	
	AssetsDatabaseVulkan::~AssetsDatabaseVulkan()
	{
		//todo: check if vk objects are disposed correctly
		textures.reset();
		texturesMemory.reset();
	}

	void AssetsDatabaseVulkan::AddTexture(VkImage& image, VkDeviceMemory& memory)
	{
		textures->push_back(image);
		texturesMemory->push_back(memory);
	}

	void AssetsDatabaseVulkan::RemoveTexture(VkImage& image, VkDeviceMemory& memory)
	{
		//todo: add an implementation
		throw std::runtime_error("Not implemented");
	}
}