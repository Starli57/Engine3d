#pragma once

#include <GLFW/glfw3.h>

#include "Architecture/Ref.h"
#include "Rendering/Vulkan/VulkanContext.h"

namespace AVulkan
{
	class ABuffer
	{
	public:
		void Create(Ref<VulkanContext> vulkanContext, uint64_t bufferSize,
			VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
		
		void Copy(Ref<VulkanContext> vulkanContext, VkBuffer& srcBuffer, VkBuffer& dstBuffer,
			VkDeviceSize& size) const;

		void Dispose(Ref<VulkanContext> vulkanContext, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

	private:
		void BindMemory(Ref<VulkanContext> vulkanContext, VkMemoryPropertyFlags& memoryFlags,
			VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	};
}
