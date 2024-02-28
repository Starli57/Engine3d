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
			VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& filePath);
		virtual ~TextureVulkan() override;

		//todo: make public getters and private setters
		Ref<VkImage> image;
		Ref<VkImageView> imageView;
		Ref<VkDeviceMemory> imageMemory;

	private:
		void CreateImage(const std::string& filePath);
		void CreateImageView();

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}