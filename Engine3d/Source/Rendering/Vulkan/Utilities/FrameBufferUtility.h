#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Models/ImageModel.h"
#include "SharedLib/Ref.h"

namespace VkUtils
{
	void CreateFrameBuffer(VkDevice& logicalDevice, VkRenderPass& renderPass, AVulkan::SwapChainData& swapChainData, Ref<AVulkan::ImageModel> depthBufferModel);
	void DisposeFrameBuffer(VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers);
}