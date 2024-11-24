#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	VkRenderPass CreateRenderPass(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<AVulkan::VulkanConfiguration> rendererConfig,
	                              const std::vector<VkAttachmentDescription>& attachments, const VkSubpassDescription& subpass);

	void DisposeRenderPass(VkDevice& logicalDevice, VkRenderPass& renderPass);

	void BeginRenderPass(std::vector<VkClearValue>& clearValues, VkFramebuffer& frameBuffer, VkRenderPass& renderPass, 
		VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent);
	void EndRenderPass(VkCommandBuffer& commandBuffer);
}