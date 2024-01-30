#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class AIndexBuffer
	{
	public:
		void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<uint32_t>& indices,
			VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const;
		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	};
}
