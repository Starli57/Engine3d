#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ABuffer
	{
	public:
		void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint64_t bufferSize,
			VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
		
		void CopyBuffer(VkDevice& logicalDevice, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer,
			VkDeviceSize& size, VkCommandPool& commandPool) const;

		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

	private:
		void BindMemory(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkMemoryPropertyFlags& memoryFlags,
			VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

		uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties) const;
	};
}
