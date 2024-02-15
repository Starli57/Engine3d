#include "Pch.h"
#include "AIndexBuffer.h"
#include "ABuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void AIndexBuffer::Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<std::vector<uint32_t>> indices,
        VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const
	{
        spdlog::info("Create Index buffer");

        VkDeviceSize bufferSize = sizeof(indices->at(0)) * indices->size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkBufferUsageFlags distUsageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryPropertyFlags distMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        ABuffer bufferInterface;
        bufferInterface.Create(physicalDevice, logicalDevice, bufferSize, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices->data(), (size_t)bufferSize);
        vkUnmapMemory(logicalDevice, stagingBufferMemory);

        bufferInterface.Create(physicalDevice, logicalDevice, bufferSize, distUsageFlags, distMemoryFlags, indexBuffer, bufferMemory);

        bufferInterface.Copy(logicalDevice, graphicsQueue, stagingBuffer, indexBuffer, bufferSize, commandPool);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
	}


    void AIndexBuffer::Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
    {
        spdlog::info("Dispose Index Buffer");
        ABuffer().Dispose(logicalDevice, buffer, bufferMemory);
    }
}