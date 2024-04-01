#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Resources/TexturesList.h"
#include "Entities/Texture.h"
#include "Rendering/Vulkan/VulkanModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "ProjectSettings.h"
#include "SharedLib/IOUtility.h"

namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(Ref<ProjectSettigns> projectSettings, Ref<VulkanModel> vulkanModel,
			VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, 
			VkSampler& textureSampler, VkCommandPool& commandPool, TextureId textureId);
		virtual ~TextureVulkan() override;

		//todo: make public getters and private setters
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;

	private:
		void CreateImage(TextureId textureId);
		void CreateImageView();

		Ref<ProjectSettigns> projectSettings;
		Ref<VulkanModel> vulkanModel;
		
	};
}