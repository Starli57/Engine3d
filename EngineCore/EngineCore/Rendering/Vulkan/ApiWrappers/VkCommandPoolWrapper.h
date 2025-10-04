#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void CreateCommandPool(const VulkanContext* vulkanContext, VkCommandPool& outCommandPool);
	void DisposeCommandPool(const VulkanContext* vulkanContext, const VkCommandPool& commandPool);
}
