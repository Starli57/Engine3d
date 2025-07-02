#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(const Ref<AVulkan::VulkanContext>& context);
	void DisposeSurface(const Ref<AVulkan::VulkanContext>& context);
}
