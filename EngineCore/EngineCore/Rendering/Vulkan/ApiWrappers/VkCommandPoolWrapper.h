#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkCommandPool CreateCommandPool(const VulkanContext* vulkanContext);
	void DisposeCommandPool(const VulkanContext* vulkanContext, const VkCommandPool& commandPool);
}
