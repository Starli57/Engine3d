#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	VkRenderPass CreateRenderPass(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<AVulkan::VulkanConfiguration> rendererConfig);
	void DisposeRenderPass(VkDevice& logicalDevice, VkRenderPass& renderPass);
}