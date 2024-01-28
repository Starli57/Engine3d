#include "Pch.h"
#include "AVertexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void AVertexBuffer::Create(VkDevice& logicalDevice, std::vector<Vertex>& vertices, 
		VkBufferCreateInfo& bufferInfo, VkBuffer& vertexBuffer) const
	{
		spdlog::info("Create Vertex Buffer");

		auto createStatus = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &vertexBuffer);
		if (createStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create vertex buffer, status: " + createStatus);
		}
	}

	void AVertexBuffer::Setup(std::vector<Vertex>& vertices, VkBufferCreateInfo& bufferInfo) const
	{
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(Vertex) * vertices.size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	void AVertexBuffer::Dispose(VkDevice& logicalDevice, VkBuffer& vertexBuffer) const
	{
		spdlog::info("Dispose Vertex Buffer");

		vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
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
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
}