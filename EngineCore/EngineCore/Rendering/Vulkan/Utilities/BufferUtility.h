#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void CreateStagingBuffer(const VulkanContext* vulkanContext, VkDeviceSize bufferSize, void const* sourceData,
	                         VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	
	void CreateDeviceLocalBuffer(const VulkanContext* vulkanContext, VkDeviceSize bufferSize, void const* sourceData, VkBufferUsageFlags distUsageFlags,
	                             VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool);
	
	void CreateBuffer(const VulkanContext* vulkanContext, uint64_t bufferSize,
	                  const VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void DisposeBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);

	void CopyBuffer(const VkDevice& logicalDevice, const VkQueue& graphicsQueue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer,
	                const VkDeviceSize& size, const VkCommandPool& commandPool);

	VkCommandBuffer BeginCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& commandPool);
	void SubmitCommandBuffer(const VkQueue& graphicsQueue, const VkCommandBuffer& commandBuffer);

	void BindMemory(const VulkanContext* vulkanContext, const VkMemoryPropertyFlags& memoryFlags,
	                const VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
