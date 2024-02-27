#pragma once

#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>

#include "Utilities/IOUtility.h"

namespace AVulkan
{
	class TextureVulkan
	{
	public: 
		size_t Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
			VkQueue& graphicsQueue, VkCommandPool& commandPool, AssetsDatabaseVulkan& assetsDatabase,
			const std::string& filePath, int& width, int& height);
		Ref<stbi_uc> Load(const std::string& filePath, int& width, int& height, VkDeviceSize* imageSize);
	};
}