#include "Pch.h"

#include "MeshVulkan.h"
#include "Rendering/Vulkan/Builders/Buffers/AVertexBuffer.h"

#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }

	uint32_t MeshVulkan::GetVertexCount() { return static_cast<uint32_t>(vertices.size()); }

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Mesh& mesh)
	{
		this->logicalDevice = &logicalDevice;
		this->vertices = mesh.GetVertices();
		
		AVertexBuffer().Create(physicalDevice, logicalDevice, vertices, vertexBuffer, vertexBufferMemory);
	}

	MeshVulkan::~MeshVulkan()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer, vertexBufferMemory);
	}
}