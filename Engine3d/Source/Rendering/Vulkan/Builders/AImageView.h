#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class AImageView
	{
	public:
		void Create(VkDevice& logicalDevice, Ref<SwapChainData> swapChainData) const;
		void Dispose(VkDevice& logicalDevice, Ref<SwapChainData> swapChainData) const;
	};
}
