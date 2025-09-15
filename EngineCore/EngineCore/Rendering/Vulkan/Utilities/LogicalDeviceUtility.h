#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkDevice CreateLogicalDevice(VulkanContext* vulkanContext);
	void DisposeLogicalDevice(const VkDevice& logicalDevice);
}
