#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	VkRenderPass CreateRenderPass(VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, Ref<AVulkan::VulkanConfiguration> rendererConfig,
		const std::vector<VkAttachmentDescription>& attachments, const VkSubpassDescription& subpass);

	void DisposeRenderPass(const VkDevice& logicalDevice, const VkRenderPass& renderPass);

	void BeginRenderPass(const std::vector<VkClearValue>& clearValues, const VkFramebuffer& frameBuffer, const VkRenderPass& renderPass,
	                     const VkCommandBuffer& commandBuffer, const VkExtent2D& vkExtent);
	void EndRenderPass(const VkCommandBuffer& commandBuffer);
}