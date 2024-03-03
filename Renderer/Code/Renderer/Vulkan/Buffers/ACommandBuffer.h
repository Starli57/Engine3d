#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Ref.h"
#include "Vulkan/Models/SwapChainData.h"
#include "Vulkan/Entities/MeshVulkan.h"
#include "Vulkan/GraphicsPipeline.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			SwapChainData& swapChainData, GraphicsPipeline& pipeline) const;
	};
}