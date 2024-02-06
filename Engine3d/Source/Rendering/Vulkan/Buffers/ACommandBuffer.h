#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Context/Device.h"
#include "Rendering/Vulkan/Model/SwapChainData.h"
#include "Rendering/Vulkan/Mesh/MeshVulkan.h"
#include "Rendering/Vulkan/Context/GraphicsPipeline.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(Device& deviceContext, uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			SwapChainData& swapChainData, GraphicsPipeline& pipeline, std::vector<MeshVulkan*>& meshes) const;
	};
}