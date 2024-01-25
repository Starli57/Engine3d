#include "Pch.h"

#include "Mesh.h"
#include "Buffers/AVertexBuffer.h"

#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer Mesh::GetVertexBuffer() { return vertexBuffer; }

	uint32_t Mesh::GetVertexCount() { return static_cast<uint32_t>(vertices->size()); }

	Mesh::Mesh(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices)
	{
		this->logicalDevice = &logicalDevice;
		this->vertices = &vertices;
		
		VkBufferCreateInfo bufferInfo{};
		AVertexBuffer().Setup(vertices, bufferInfo);
		AVertexBuffer().Create(logicalDevice, vertices, bufferInfo, vertexBuffer);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		auto allocateStatus = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &vertexBufferMemory);
		if (allocateStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Can't allocate memory for vertex buffer, status: " + allocateStatus);
		}

		vkBindBufferMemory(logicalDevice, vertexBuffer, vertexBufferMemory, 0);

		void* data;
		vkMapMemory(logicalDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferInfo.size);
		vkUnmapMemory(logicalDevice, vertexBufferMemory);
	}

	Mesh::~Mesh()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer);
		vkFreeMemory(*logicalDevice, vertexBufferMemory, nullptr);
	}

	uint32_t Mesh::FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		spdlog::info("Available memory types coint: {0}", memProperties.memoryTypeCount);

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