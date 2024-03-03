#pragma once

#include <GLFW/glfw3.h>

#include "Renderer/Model/UboViewProjection.h"
#include "Renderer/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout, VkImageView& textureImageView, VkSampler& textureSampler) const;
	};
}