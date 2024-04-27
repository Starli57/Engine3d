#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Models/ImageModel.h"
#include "SharedLib/Ref.h"

namespace AVulkan
{
	class AFrameBuffer
	{
	public:
		void Create(VkDevice& logicalDevice, VkRenderPass& renderPass, SwapChainData& swapChainData, Ref<ImageModel> depthBufferModel) const;
		void Dispose(VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers) const;
	};
}