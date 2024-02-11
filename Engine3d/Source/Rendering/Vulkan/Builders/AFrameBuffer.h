#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Model/SwapChainData.h"

namespace AVulkan
{
	class AFrameBuffer
	{
	public:
		void Create(VkDevice& logicalDevice, VkRenderPass& renderPass, SwapChainData& swapChainData) const;
		void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
	};
}