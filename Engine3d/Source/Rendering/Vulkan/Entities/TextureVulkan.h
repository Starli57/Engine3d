#pragma once

#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>

#include "Rendering/Entity/Texture.h"
#include "Utilities/IOUtility.h"

namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
			VkQueue& graphicsQueue, VkCommandPool& commandPool, AssetsDatabaseVulkan& assetsDatabase,
			const std::string& filePath, int& width, int& height);
		virtual ~TextureVulkan() override;

		const std::string& path;

		//todo: make public getters and private setters
		Ref<VkImage> texture;
		Ref<VkDeviceMemory> textureMemory;

	private:
		Ref<stbi_uc> Load(const std::string& filePath, int& width, int& height, VkDeviceSize* imageSize);
		void Create(VkPhysicalDevice& physicalDevice,
			VkQueue& graphicsQueue, VkCommandPool& commandPool, AssetsDatabaseVulkan& assetsDatabase,
			const std::string& filePath, int& width, int& height);

		VkDevice& logicalDevice;
	};
}