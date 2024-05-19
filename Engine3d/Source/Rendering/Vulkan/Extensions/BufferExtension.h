#pragma once

#include <GLFW/glfw3.h>

namespace BufferExtension
{
	void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint64_t bufferSize,
		VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		
	void Copy(VkDevice& logicalDevice, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer,
		VkDeviceSize& size, VkCommandPool& commandPool);

	void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkCommandBuffer BeginCommandBuffer(VkDevice& logicalDevice, VkCommandPool& commandPool);
	void SubmitCommandBuffer(VkQueue& graphicsQueue, VkCommandBuffer& commandBuffer);

	void BindMemory(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkMemoryPropertyFlags& memoryFlags,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
