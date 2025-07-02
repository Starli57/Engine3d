#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VkUtils
{
	VkDevice CreateLogicalDevice(const Ref<AVulkan::VulkanContext>& context);
	void DisposeLogicalDevice(const VkDevice& logicalDevice);
}
