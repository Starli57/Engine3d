#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Model/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorPool
	{
	public:
		void Create(VkDevice& logicalDevice, SwapChainData& swapChainData, 
			uint32_t vpUniformBufferSize, uint32_t modelUniformBufferSize,
			VkDescriptorPool& descriptorPool) const;
		void Dispose(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
	};

}