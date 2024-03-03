#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Entities/Texture.h"
#include "Vulkan/Models/SwapChainData.h"
#include "SharedLib/IOUtility.h"

namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, SwapChainData& swapChainData,
			VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, VkSampler& textureSampler,
			VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& filePath);
		virtual ~TextureVulkan() override;

		//todo: make public getters and private setters
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;

	private:
		void CreateImage(const std::string& filePath);
		void CreateImageView();

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}