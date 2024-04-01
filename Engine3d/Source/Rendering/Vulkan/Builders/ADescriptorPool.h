#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class ADescriptorPool
	{
	public:
		void Create(VkDevice& logicalDevice, Ref<SwapChainData> swapChainData, VkDescriptorPool& descriptorPool) const;
		void Dispose(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
	};

}