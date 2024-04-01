#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Rendering/Vulkan/VulkanModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(Ref<VulkanModel> vulkanModel, 
			VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout,
			VkImageView& textureImageView, VkSampler& textureSampler) const;
	};
}