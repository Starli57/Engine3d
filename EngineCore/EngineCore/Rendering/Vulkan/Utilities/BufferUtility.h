#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void CreateStagingBuffer(VkDeviceSize bufferSize, void const* sourceData,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory, const Ref<AVulkan::VulkanContext>& context);
	
	void CreateDeviceLocalBuffer(VkDeviceSize bufferSize, void const* sourceData, VkBufferUsageFlags distUsageFlags,
		const Ref<AVulkan::VulkanContext>& context, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool);
	
	void CreateBuffer(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, uint64_t bufferSize,
	                  const VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void DisposeBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);

	void CopyBuffer(const VkDevice& logicalDevice, const VkQueue& graphicsQueue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer,
	                const VkDeviceSize& size, const VkCommandPool& commandPool);

	VkCommandBuffer BeginCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& commandPool);
	void SubmitCommandBuffer(const VkQueue& graphicsQueue, const VkCommandBuffer& commandBuffer);

	void BindMemory(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, const VkMemoryPropertyFlags& memoryFlags,
	                const VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
