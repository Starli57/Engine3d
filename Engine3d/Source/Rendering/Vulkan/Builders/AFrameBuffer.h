#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Data/SwapChainData.h"
#include "Rendering/Vulkan/Data/DepthBufferModel.h"
#include "Architecture/Ref.h"

namespace AVulkan
{
	class AFrameBuffer
	{
	public:
		void Create(VkDevice& logicalDevice, VkRenderPass& renderPass, SwapChainData& swapChainData, Ref<DepthBufferModel> depthBufferModel) const;
		void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
	};
}