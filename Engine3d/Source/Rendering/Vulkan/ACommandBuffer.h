#pragma once

#include <GLFW/glfw3.h>

#include "SwapChainData.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(VkCommandBuffer& commandBuffer, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			VkExtent2D& swapchainExtent, VkPipeline& pipeline) const;
	};
}