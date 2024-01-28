#include "Pch.h"

#include "MeshVulkan.h"
#include "Rendering/Vulkan/Builders/Buffers/AVertexBuffer.h"

#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }

	uint32_t MeshVulkan::GetVertexCount() { return static_cast<uint32_t>(vertices.size()); }

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, Mesh& mesh)
	{
		this->logicalDevice = &logicalDevice;
		this->vertices = mesh.GetVertices();
		
		AVertexBuffer().Create(physicalDevice, logicalDevice, vertices, vertexBuffer, vertexBufferMemory, 
			graphicsQueue, commandPool);
	}

	MeshVulkan::~MeshVulkan()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer, vertexBufferMemory);
	}
}