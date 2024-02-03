#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Data/Mvp.h"
#include "Rendering/Vulkan/Data/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout) const;
	};
}