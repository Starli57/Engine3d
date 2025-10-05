#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
	void CreateFrameBuffer(const VkDevice& logicalDevice, const VkRenderPass& renderPass,
	                       int width, int height, const std::vector<VkImageView>& attachments, VkFramebuffer& frameBuffer);

	void DisposeFrameBuffer(const VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers);
	void DisposeFrameBuffer(const VkDevice& logicalDevice, const VkFramebuffer& frameBuffer);
}