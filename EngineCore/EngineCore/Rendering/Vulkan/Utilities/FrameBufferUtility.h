#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	void CreateFrameBuffer(const VkDevice& logicalDevice, const VkRenderPass& renderPass,
	                       const VkExtent2D& extent, const std::vector<VkImageView>& attachments, VkFramebuffer& frameBuffer);

	void DisposeFrameBuffer(const VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers);
	void DisposeFrameBuffer(const VkDevice& logicalDevice, const VkFramebuffer& frameBuffer);
}