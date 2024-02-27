#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Architecture/Ref.h"

namespace AVulkan
{
	class AssetsDatabaseVulkan
	{
	public:
		URef<std::vector<VkImage>> textures;
		URef<std::vector<VkDeviceMemory>> texturesMemory;

		AssetsDatabaseVulkan();
		~AssetsDatabaseVulkan();

		void AddTexture(VkImage& image, VkDeviceMemory& memory);
		void RemoveTexture(VkImage& image, VkDeviceMemory& memory);
	};
}