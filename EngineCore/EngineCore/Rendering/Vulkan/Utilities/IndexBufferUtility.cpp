#include "EngineCore/Pch.h"

#include "spdlog/spdlog.h"

#include "IndexBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"

namespace VkUtils
{
	void CreateIndexBuffer(const Ref<AVulkan::VulkanContext>& context, std::vector<uint32_t>& indices,
        VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool)
	{
        VkDeviceSize bufferSize = sizeof(indices.at(0)) * indices.size();
        VkBufferUsageFlags distUsageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		CreateDeviceLocalBuffer(bufferSize, indices.data(), distUsageFlags, context, indexBuffer, bufferMemory, commandPool);
	}


    void DisposeIndexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
    {
		if (buffer == VK_NULL_HANDLE) return;
        VkUtils::DisposeBuffer(logicalDevice, buffer, bufferMemory);
    }
}