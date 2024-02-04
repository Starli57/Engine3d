#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Model/UboViewProjection.h"
#include "Rendering/Vulkan/Model/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout) const;
	};
}