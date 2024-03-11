#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Resources/TexturesList.h"
#include "Entities/Texture.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "SharedLib/ProjectSettings.h"
#include "SharedLib/IOUtility.h"

namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(Ref<ProjectSettigns> projectSettings, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, SwapChainData& swapChainData,
			VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, VkSampler& textureSampler,
			VkQueue& graphicsQueue, VkCommandPool& commandPool, TextureId textureId);
		virtual ~TextureVulkan() override;

		//todo: make public getters and private setters
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;

	private:
		void CreateImage();
		void CreateImageView();

		Ref<ProjectSettigns> projectSettings;

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}