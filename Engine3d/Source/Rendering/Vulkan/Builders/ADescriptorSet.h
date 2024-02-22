#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Model/Mvp.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout) const;
	};
}