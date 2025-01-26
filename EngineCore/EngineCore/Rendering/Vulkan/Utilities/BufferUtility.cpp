#include "EngineCore/Pch.h"
#include "BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"

namespace VkUtils
{
	void CreateBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const uint64_t bufferSize,
	                  const VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto createStatus = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create buffer, status: " + createStatus);
	
		BindMemory(physicalDevice, logicalDevice, memoryFlags, buffer, bufferMemory);
	}

	void DisposeBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
	{
		vkDestroyBuffer(logicalDevice, buffer, nullptr);
		vkFreeMemory(logicalDevice, bufferMemory, nullptr);
	}

	void CopyBuffer(VkDevice& logicalDevice, VkQueue& graphicsQueue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer,
	                const VkDeviceSize& size, VkCommandPool& commandPool)
	{
		auto commandBuffer = BeginCommandBuffer(logicalDevice, commandPool);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		SubmitCommandBuffer(graphicsQueue, commandBuffer);

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}

	VkCommandBuffer BeginCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void SubmitCommandBuffer(const VkQueue& graphicsQueue, const VkCommandBuffer& commandBuffer)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);
	}


	void BindMemory(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, const VkMemoryPropertyFlags& memoryFlags,
	                const VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryFlags);

		const auto allocateStatus = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Can't allocate memory for vertex buffer, status: " + allocateStatus);

		vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
	}
}