#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	VkCommandPool CreateCommandPool(const Ref<AVulkan::VulkanContext>& context);
	void DisposeCommandPool(const Ref<AVulkan::VulkanContext>& context, const VkCommandPool& commandPool);
}
