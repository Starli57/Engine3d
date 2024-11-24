#include "EngineCore/Pch.h"

#include "MeshVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/VertexBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/IndexBufferUtility.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkBuffer MeshVulkan::GetVertexBuffer() { return vertexBuffer; }
	VkBuffer MeshVulkan::GetIndexBuffer()  { return indexBuffer; }

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
		VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::filesystem::path& path) : Mesh(path),
		physicalDevice(physicalDevice), logicalDevice(logicalDevice)
	{
		CreateVertexBuffer(graphicsQueue, commandPool);
		CreateIndexBuffer(graphicsQueue, commandPool);
	}

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
		VkQueue& graphicsQueue, VkCommandPool& commandPool, 
		Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) : Mesh(vertices, indices),
		physicalDevice(physicalDevice), logicalDevice(logicalDevice)
	{
		CreateVertexBuffer(graphicsQueue, commandPool);
		CreateIndexBuffer(graphicsQueue, commandPool);
	}

	MeshVulkan::~MeshVulkan()
	{
		spdlog::info("Dispose Index Buffer");
		VkUtils::DisposeIndexBuffer(logicalDevice, indexBuffer, indexBufferMemory);

		spdlog::info("Dispose Vertex Buffer");
		VkUtils::DisposeVertexBuffer(logicalDevice, vertexBuffer, vertexBufferMemory);
	}

	void MeshVulkan::CreateVertexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool)
	{
		VkUtils::CreateVertexBuffer(physicalDevice, logicalDevice,
			vertices, vertexBuffer, vertexBufferMemory, graphicsQueue, commandPool);
	}

	void MeshVulkan::CreateIndexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool)
	{
		VkUtils::CreateIndexBuffer(physicalDevice, logicalDevice,
			indices, indexBuffer, indexBufferMemory, graphicsQueue, commandPool);
	}
}