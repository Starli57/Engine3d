#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	void CreateBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint64_t bufferSize,
	                  const VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void DisposeBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);

	void CopyBuffer(VkDevice& logicalDevice, VkQueue& graphicsQueue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer,
	                const VkDeviceSize& size, VkCommandPool& commandPool);

	VkCommandBuffer BeginCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& commandPool);
	void SubmitCommandBuffer(const VkQueue& graphicsQueue, const VkCommandBuffer& commandBuffer);

	void BindMemory(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, const VkMemoryPropertyFlags& memoryFlags,
	                const VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
