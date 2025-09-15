#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkRenderPass CreateRenderPass(VulkanContext* vulkanContext,
		const std::vector<VkAttachmentDescription>& attachments, const VkSubpassDescription& subpass);

	void DisposeRenderPass(const VkDevice& logicalDevice, const VkRenderPass& renderPass);

	void BeginRenderPass(const std::vector<VkClearValue>& clearValues, const VkFramebuffer& frameBuffer, const VkRenderPass& renderPass,
	                     const VkCommandBuffer& commandBuffer, const VkExtent2D& vkExtent);
	void EndRenderPass(const VkCommandBuffer& commandBuffer);
}
