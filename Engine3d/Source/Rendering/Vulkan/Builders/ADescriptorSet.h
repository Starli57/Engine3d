#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, std::vector<VkDescriptorSet>& descriptorSets, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout, VkImageView& textureImageView, VkSampler& textureSampler) const;
	};
}