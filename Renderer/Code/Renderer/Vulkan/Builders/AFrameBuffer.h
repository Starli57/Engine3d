#pragma once

#include <GLFW/glfw3.h>
#include "Renderer/Vulkan/Models/SwapChainData.h"
#include "Renderer/Vulkan/Models/DepthBufferModel.h"
#include "SharedLib/Ref.h"

namespace AVulkan
{
	class AFrameBuffer
	{
	public:
		void Create(VkDevice& logicalDevice, VkRenderPass& renderPass, SwapChainData& swapChainData, Ref<DepthBufferModel> depthBufferModel) const;
		void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
	};
}