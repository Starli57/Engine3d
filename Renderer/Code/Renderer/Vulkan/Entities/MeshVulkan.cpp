#include "Pch.h"

#include "MeshVulkan.h"
#include "Renderer/Vulkan/Buffers/AVertexBuffer.h"
#include "Renderer/Vulkan/Buffers/AIndexBuffer.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }
	VkBuffer MeshVulkan::GetIndexBuffer()  { return indexBuffer; }

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, SwapChainData& swapChainData, 
		VkQueue& graphicsQueue, VkCommandPool& commandPool, 
		Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) : Mesh(vertices, indices)
	{
		this->logicalDevice = &logicalDevice;

		AVertexBuffer().Create(physicalDevice, logicalDevice, vertices, vertexBuffer, vertexBufferMemory, 
			graphicsQueue, commandPool);
		AIndexBuffer().Create(physicalDevice, logicalDevice, indices, indexBuffer, indexBufferMemory,
			graphicsQueue, commandPool);

	}

	MeshVulkan::~MeshVulkan()
	{
		AVertexBuffer().Dispose(*logicalDevice, vertexBuffer, vertexBufferMemory);
		AIndexBuffer().Dispose(*logicalDevice, indexBuffer, indexBufferMemory);
	}
}