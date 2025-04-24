#include "EngineCore/Pch.h"

#include "spdlog/spdlog.h"

#include "IndexBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"

namespace VkUtils
{
	void CreateIndexBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<uint32_t>& indices,
        VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool)
	{
        spdlog::info("Create Index buffer");

        VkDeviceSize bufferSize = sizeof(indices.at(0)) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkBufferUsageFlags distUsageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryPropertyFlags distMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkUtils::CreateBuffer(physicalDevice, logicalDevice, bufferSize, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(logicalDevice, stagingBufferMemory);

        VkUtils::CreateBuffer(physicalDevice, logicalDevice, bufferSize, distUsageFlags, distMemoryFlags, indexBuffer, bufferMemory);
        VkUtils::CopyBuffer(logicalDevice, graphicsQueue, stagingBuffer, indexBuffer, bufferSize, commandPool);
        VkUtils::DisposeBuffer(logicalDevice, stagingBuffer, stagingBufferMemory);
	}


    void DisposeIndexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
    {
		if (buffer == VK_NULL_HANDLE) return;
        spdlog::info("Dispose Index Buffer");
        VkUtils::DisposeBuffer(logicalDevice, buffer, bufferMemory);
    }
}