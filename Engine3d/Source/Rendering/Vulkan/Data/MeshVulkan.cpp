#include "Pch.h"

#include "MeshVulkan.h"
#include "Rendering/Vulkan/Buffers/AVertexBuffer.h"
#include "Rendering/Vulkan/Buffers/AIndexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }
	VkBuffer MeshVulkan::GetIndexBuffer()  { return indexBuffer; }

	uint32_t MeshVulkan::GetVertexCount() { return static_cast<uint32_t>(vertices->size()); }
	uint32_t MeshVulkan::GetIndicesCount() { return static_cast<uint32_t>(indices->size()); }

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, Mesh& mesh)
	{
		this->logicalDevice = &logicalDevice;
		this->vertices = mesh.GetVertices();
		this->indices = mesh.GetIndices();

		AVertexBuffer().Create(physicalDevice, logicalDevice, *vertices, vertexBuffer, vertexBufferMemory, 
			graphicsQueue, commandPool);
		AIndexBuffer().Create(physicalDevice, logicalDevice, *indices, indexBuffer, indexBufferMemory,
			graphicsQueue, commandPool);
	}

	MeshVulkan::~MeshVulkan()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer, vertexBufferMemory);
		AIndexBuffer().Dispose(*logicalDevice, indexBuffer, indexBufferMemory);
	}
}