#include "Pch.h"

#include "MeshVulkan.h"
#include "Rendering/Vulkan/Buffers/AVertexBuffer.h"
#include "Rendering/Vulkan/Buffers/AIndexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }
	VkBuffer MeshVulkan::GetIndexBuffer()  { return indexBuffer; }

	//todo: reduce duplications in constructors
	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
		VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& path) : Mesh(path)
	{
		this->logicalDevice = &logicalDevice;

		AVertexBuffer().Create(physicalDevice, logicalDevice, 
			vertices, vertexBuffer, vertexBufferMemory,
			graphicsQueue, commandPool);
		
		AIndexBuffer().Create(physicalDevice, logicalDevice, 
			indices, indexBuffer, indexBufferMemory,
			graphicsQueue, commandPool);
	}

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
		VkQueue& graphicsQueue, VkCommandPool& commandPool, 
		Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) : Mesh(vertices, indices)
	{
		this->logicalDevice = &logicalDevice;

		AVertexBuffer().Create(physicalDevice, logicalDevice, 
			vertices, vertexBuffer, vertexBufferMemory, 
			graphicsQueue, commandPool);

		AIndexBuffer().Create(physicalDevice, logicalDevice, 
			indices, indexBuffer, indexBufferMemory,
			graphicsQueue, commandPool);

	}

	MeshVulkan::~MeshVulkan()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer, vertexBufferMemory);
		AIndexBuffer().Dispose(*logicalDevice, indexBuffer, indexBufferMemory);
	}
}