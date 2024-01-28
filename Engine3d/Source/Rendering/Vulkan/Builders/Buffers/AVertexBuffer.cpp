#include "Pch.h"
#include "ABuffer.h"
#include "AVertexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void AVertexBuffer::Create(VkPhysicalDevice physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		spdlog::info("Create Vertex Buffer");

		uint64_t bufferSize = sizeof(Vertex) * vertices.size();
		VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		ABuffer().Create(physicalDevice, logicalDevice, bufferSize, usageFlags, memoryPropertyFlags, buffer, bufferMemory);

		void* data;
		vkMapMemory(logicalDevice, bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(logicalDevice, bufferMemory);
	}

	void AVertexBuffer::Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
		spdlog::info("Dispose Vertex Buffer");
		ABuffer().Dispose(logicalDevice, buffer, bufferMemory);
	}

	VkVertexInputBindingDescription AVertexBuffer::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 2> AVertexBuffer::GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
}