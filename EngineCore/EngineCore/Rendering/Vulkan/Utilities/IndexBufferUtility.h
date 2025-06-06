#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	void CreateIndexBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<uint32_t>& indices,
		VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool);
	void DisposeIndexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);
}
