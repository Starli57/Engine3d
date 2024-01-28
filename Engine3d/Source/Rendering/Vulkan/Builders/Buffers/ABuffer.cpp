#include "Pch.h"
#include "ABuffer.h"

namespace AVulkan
{
	void ABuffer::Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint64_t bufferSize, 
		VkBufferUsageFlags& usageFlags, VkMemoryPropertyFlags memoryFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto createStatus = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer);
		if (createStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create buffer, status: " + createStatus);
		}

		BindMemory(physicalDevice, logicalDevice, memoryFlags, buffer, bufferMemory);
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
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryFlags);

		auto allocateStatus = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory);
		if (allocateStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Can't allocate memory for vertex buffer, status: " + allocateStatus);
		}

		vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
	}

	uint32_t ABuffer::FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties) const
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");
	}
}