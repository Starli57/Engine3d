#include "Pch.h"
#include "Rendering/Vulkan/Utilities/VkBufferUtility.h"
#include "AVertexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void AVertexBuffer::Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<std::vector<Vertex>> vertices, 
		VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const
	{
		spdlog::info("Create Vertex Buffer");

		uint64_t bufferSize = sizeof(Vertex) * vertices->size();

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkBufferUsageFlags distUsageFlags    = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VkMemoryPropertyFlags distMemoryFlags    = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;

		VkUtils::CreateBuffer(physicalDevice, logicalDevice, bufferSize,
			stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(logicalDevice, stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices->data(), (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, stagingMemory);

		VkUtils::CreateBuffer(physicalDevice, logicalDevice, bufferSize,
			distUsageFlags, distMemoryFlags, vertexBuffer, bufferMemory);

		VkUtils::CopyBuffer(logicalDevice, graphicsQueue, stagingBuffer, vertexBuffer, bufferSize, commandPool);
		VkUtils::DisposeBuffer(logicalDevice, stagingBuffer, stagingMemory);
	}

	void AVertexBuffer::Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		spdlog::info("Dispose Vertex Buffer");
		VkUtils::DisposeBuffer(logicalDevice, buffer, bufferMemory);
	}

	VkVertexInputBindingDescription AVertexBuffer::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 3> AVertexBuffer::GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, uv);

		return attributeDescriptions;
	}
}