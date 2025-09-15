#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkDevice CreateLogicalDevice(const Ref<VulkanContext>& context);
	void DisposeLogicalDevice(const VkDevice& logicalDevice);
}
