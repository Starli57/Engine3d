#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Data/SwapChainData.h"
#include "Rendering/Vulkan/Data/MeshVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			SwapChainData& swapChainData, GraphicsPipeline& pipeline, std::vector<MeshVulkan*>& meshes) const;
	};
}