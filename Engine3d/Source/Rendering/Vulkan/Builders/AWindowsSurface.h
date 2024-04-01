#pragma once

#include <GLFW/glfw3.h>
#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/VulkanModel.h"

namespace AVulkan
{
	class AWindowsSurface
	{
	public:
		VkSurfaceKHR Create(Ref<VulkanModel> vulkanModel) const;
		void Dispose(Ref<VulkanModel> vulkanModel) const;
	};
}