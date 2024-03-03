#pragma once

#include <GLFW/glfw3.h>

#include "Renderer/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorPool
	{
	public:
		void Create(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool) const;
		void Dispose(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
	};

}