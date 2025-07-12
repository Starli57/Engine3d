#include "EngineCore/Pch.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "VertexBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
	void CreateVertexBuffer(const Ref<AVulkan::VulkanContext>& context, std::vector<Vertex>& vertices,
		VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool)
	{
		//todo: use BufferUtility::CreateDeviceLocalBuffer instead of code below
		uint64_t bufferSize = sizeof(Vertex) * vertices.size();

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkBufferUsageFlags distUsageFlags    = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VkMemoryPropertyFlags distMemoryFlags    = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;

		CreateBuffer(context->physicalDevice, context->logicalDevice, bufferSize,
			stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(context->logicalDevice, stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(context->logicalDevice, stagingMemory);

		CreateBuffer(context->physicalDevice, context->logicalDevice, bufferSize,
			distUsageFlags, distMemoryFlags, vertexBuffer, bufferMemory);

		CopyBuffer(context->logicalDevice, context->graphicsQueue, stagingBuffer, vertexBuffer, bufferSize, commandPool);
		DisposeBuffer(context->logicalDevice, stagingBuffer, stagingMemory);
	}

	void DisposeVertexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
	{
		if (buffer == VK_NULL_HANDLE) return;
		DisposeBuffer(logicalDevice, buffer, bufferMemory);
	}

	VkVertexInputBindingDescription GetVertexInputBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 6> GetVertexInputAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, tangent);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, bitangent);

		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[4].offset = offsetof(Vertex, uv);

		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
}