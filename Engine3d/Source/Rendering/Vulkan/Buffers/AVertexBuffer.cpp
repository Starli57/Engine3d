#include "Pch.h"
#include "AVertexBuffer.h"


namespace AVulkan
{
	void AVertexBuffer::Create(VkDevice& logicalDevice, std::vector<Vertex>& vertices, 
		VkBufferCreateInfo& bufferInfo, VkBuffer& vertexBuffer) const
	{
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
		vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	}
}