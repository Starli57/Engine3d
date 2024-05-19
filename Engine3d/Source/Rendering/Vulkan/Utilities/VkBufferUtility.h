#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	void CreateBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint64_t bufferSize,
		VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void DisposeBuffer(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void CopyBuffer(VkDevice& logicalDevice, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer,
		VkDeviceSize& size, VkCommandPool& commandPool);

	VkCommandBuffer BeginCommandBuffer(VkDevice& logicalDevice, VkCommandPool& commandPool);
	void SubmitCommandBuffer(VkQueue& graphicsQueue, VkCommandBuffer& commandBuffer);

	void BindMemory(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkMemoryPropertyFlags& memoryFlags,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
