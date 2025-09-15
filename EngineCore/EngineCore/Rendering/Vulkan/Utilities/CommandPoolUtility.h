#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkCommandPool CreateCommandPool(VulkanContext* vulkanContext);
	void DisposeCommandPool(VulkanContext* vulkanContext, const VkCommandPool& commandPool);
}
