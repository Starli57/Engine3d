#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Model/UboViewProjection.h"
#include "Rendering/Vulkan/Model/SwapChainData.h"
#include "Rendering/Vulkan/Context/Device.h"

namespace AVulkan
{
	class ADescriptorSet
	{
	public:
		void Allocate(Device& deviceContext, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
			VkDescriptorSetLayout& descriptorSetLayout) const;
	};
}