#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Ref.h"
#include "Renderer/Model/RenderModel.h"
#include "Renderer/Vulkan/Models/SwapChainData.h"
#include "Renderer/Vulkan/Entities/MeshVulkan.h"
#include "Renderer/Vulkan/GraphicsPipeline.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(std::vector<RenderModel>& renderModels, uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			SwapChainData& swapChainData, GraphicsPipeline& pipeline) const;
	};
}