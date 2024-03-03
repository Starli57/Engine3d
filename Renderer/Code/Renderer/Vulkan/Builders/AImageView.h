#pragma once

#include <GLFW/glfw3.h>

#include "Renderer/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class AImageView
	{
	public:
		void Create(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
		void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
	};
}
