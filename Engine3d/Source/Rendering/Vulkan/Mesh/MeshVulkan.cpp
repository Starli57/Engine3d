#include "Pch.h"

#include "MeshVulkan.h"
#include "Rendering/Vulkan/Buffers/AVertexBuffer.h"
#include "Rendering/Vulkan/Buffers/AIndexBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }
	VkBuffer MeshVulkan::GetIndexBuffer()  { return indexBuffer; }

	MeshVulkan::MeshVulkan(Ref<VulkanContext> vulkanContext,
		Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) : Mesh(vertices, indices)
	{
		this->vulkanContext = vulkanContext;

		AVertexBuffer().Create(physicalDevice, logicalDevice, vertices, vertexBuffer, vertexBufferMemory, 
			graphicsQueue, commandPool);
		AIndexBuffer().Create(physicalDevice, logicalDevice, indices, indexBuffer, indexBufferMemory,
			graphicsQueue, commandPool);

	}

	MeshVulkan::~MeshVulkan()
	{
		auto logicalDevice = vulkanContext->GetVkLogicalDevice();
		AVertexBuffer().Dispose(logicalDevice, vertexBuffer, vertexBufferMemory);
		AIndexBuffer().Dispose(logicalDevice, indexBuffer, indexBufferMemory);
	}
}