#include "EngineCore/Pch.h"
#include "VkBufferWrapper.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/EngineApi.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkDeviceMemoryWrapper.h"

namespace VulkanApi
{
	void CreateStagingBuffer(const VulkanContext* vulkanContext, VkDeviceSize bufferSize, void const* sourceData,
	                         VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferUsageFlags usageFlagsStaging = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		constexpr VkMemoryPropertyFlags memoryFlagsStaging = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		CreateBuffer(vulkanContext, bufferSize, usageFlagsStaging, memoryFlagsStaging, buffer, bufferMemory);

		void* data;
		vkMapMemory(vulkanContext->logicalDevice, bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, sourceData, bufferSize);
		vkUnmapMemory(vulkanContext->logicalDevice, bufferMemory);	
	}
	
	void CreateDeviceLocalBuffer(const VulkanContext* vulkanContext, VkDeviceSize bufferSize, void const* sourceData, VkBufferUsageFlags distUsageFlags,
	                             VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VkMemoryPropertyFlags distMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		CreateBuffer(vulkanContext, bufferSize, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(vulkanContext->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, sourceData, (size_t)bufferSize);
		vkUnmapMemory(vulkanContext->logicalDevice, stagingBufferMemory);

		CreateBuffer(vulkanContext, bufferSize, distUsageFlags, distMemoryFlags, buffer, bufferMemory);
		CopyBuffer(vulkanContext->logicalDevice, vulkanContext->graphicsQueue, stagingBuffer, buffer, bufferSize, commandPool);
		DisposeBuffer(vulkanContext->logicalDevice, stagingBuffer, stagingBufferMemory);
	}
	
	void CreateBuffer(const VulkanContext* vulkanContext, const uint64_t bufferSize,
	                  const VkBufferUsageFlags& usageFlags, const VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto createStatus = vkCreateBuffer(vulkanContext->logicalDevice, &bufferInfo, nullptr, &buffer);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create buffer, status: " + createStatus);
	
		BindMemory(vulkanContext, memoryFlags, buffer, bufferMemory);
	}

	void DisposeBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
	{
		if (buffer != VK_NULL_HANDLE) vkDestroyBuffer(logicalDevice, buffer, nullptr);
		if (bufferMemory != VK_NULL_HANDLE) vkFreeMemory(logicalDevice, bufferMemory, nullptr);
	}

	void CopyBuffer(const VkDevice& logicalDevice, const VkQueue& graphicsQueue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer,
	                const VkDeviceSize& size, const VkCommandPool& commandPool)
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


	void BindMemory(const VulkanContext* vulkanContext, const VkMemoryPropertyFlags& memoryFlags,
	                const VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanContext->logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(vulkanContext->physicalDevice, memRequirements.memoryTypeBits, memoryFlags);

		const auto allocateStatus = vkAllocateMemory(vulkanContext->logicalDevice, &allocInfo, nullptr, &bufferMemory);
		Engine::CAssert::Check(allocateStatus == VK_SUCCESS, "Can't allocate memory for vertex buffer, status: " + allocateStatus);

		vkBindBufferMemory(vulkanContext->logicalDevice, buffer, bufferMemory, 0);
	}
}