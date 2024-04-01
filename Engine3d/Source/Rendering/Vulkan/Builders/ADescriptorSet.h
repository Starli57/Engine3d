#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, std::vector<VkDescriptorSet>& descriptorSets, std::vector<UniformBufferVulkan*>& uniformBuffers, 
			VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, VkImageView& textureImageView, VkSampler& textureSampler) const;
	};
}