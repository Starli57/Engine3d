#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VkUtils
{
	void CreateIndexBuffer(const Ref<AVulkan::VulkanContext>& context, std::vector<uint32_t>& indices,
		VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool);
	void DisposeIndexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);
}
