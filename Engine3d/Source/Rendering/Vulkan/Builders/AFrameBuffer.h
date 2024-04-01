#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/VulkanModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Models/DepthBufferModel.h"
#include "SharedLib/Ref.h"

namespace AVulkan
{
	class AFrameBuffer
	{
	public:
		void Create(Ref<VulkanModel> vulkanModel) const;
		void Dispose(Ref<VulkanModel> vulkanModel) const;
	};
}