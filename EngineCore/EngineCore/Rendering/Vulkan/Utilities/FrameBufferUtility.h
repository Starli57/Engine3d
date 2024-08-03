#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	void CreateFrameBuffer(VkDevice& logicalDevice, VkRenderPass& renderPass, AVulkan::SwapChainData& swapChainData, Ref<AVulkan::ImageModel> depthBufferModel);
	void DisposeFrameBuffer(VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers);
}