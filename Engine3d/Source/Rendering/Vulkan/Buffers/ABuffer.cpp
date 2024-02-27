#include "Pch.h"
#include "ABuffer.h"
#include "Rendering/Vulkan/Utilities/VkMemoryUtility.h"

namespace AVulkan
{
	void ABuffer::Create(Ref<VulkanContext> vulkanContext, uint64_t bufferSize,
		VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto createStatus = vkCreateBuffer(*vulkanContext->GetVkLogicalDevice().get(), &bufferInfo, nullptr, &buffer);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create buffer, status: " + createStatus);
	
		BindMemory(vulkanContext, memoryFlags, buffer, bufferMemory);
	}

	void ABuffer::Copy(Ref<VulkanContext> vulkanContext, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size) const
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = *vulkanContext->GetVkCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(*vulkanContext->GetVkLogicalDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		auto graphicsQueue = vulkanContext->GetVkGraphicsQueue();
		vkQueueSubmit(*graphicsQueue.get(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(*graphicsQueue.get());

		vkFreeCommandBuffers(*vulkanContext->GetVkLogicalDevice(), *vulkanContext->GetVkCommandPool(), 1, &commandBuffer);
	}

	void ABuffer::Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		vkDestroyBuffer(logicalDevice, buffer, nullptr);
		vkFreeMemory(logicalDevice, bufferMemory, nullptr);
	}


	void ABuffer::BindMemory(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkMemoryPropertyFlags& memoryFlags, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VkMemoryUtility::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryFlags);

		auto allocateStatus = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Can't allocate memory for vertex buffer, status: " + allocateStatus);

		vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
	}
}