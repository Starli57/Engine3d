#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	void CreateFrameBuffer(VkDevice& logicalDevice, VkRenderPass& renderPass,
		VkExtent2D& extent, std::vector<VkImageView> attachments, VkFramebuffer& frameBuffer);

	void DisposeFrameBuffer(VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers);
	void DisposeFrameBuffer(VkDevice& logicalDevice, VkFramebuffer& frameBuffer);
}