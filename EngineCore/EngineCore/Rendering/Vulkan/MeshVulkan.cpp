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
		VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::filesystem::path& path, Ref<Rollback> rollback) : Mesh(path),
		physicalDevice(physicalDevice), logicalDevice(logicalDevice)
	{
		CreateVertexBuffer(graphicsQueue, commandPool, rollback);
		CreateIndexBuffer(graphicsQueue, commandPool, rollback);
	}

	MeshVulkan::MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
		VkQueue& graphicsQueue, VkCommandPool& commandPool, 
		Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, Ref<Rollback> rollback) : Mesh(vertices, indices),
		physicalDevice(physicalDevice), logicalDevice(logicalDevice)
	{
		CreateVertexBuffer(graphicsQueue, commandPool, rollback);
		CreateIndexBuffer(graphicsQueue, commandPool, rollback);
	}

	MeshVulkan::~MeshVulkan(){}

	void MeshVulkan::CreateVertexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<Rollback> rollback)
	{
		VkUtils::CreateVertexBuffer(physicalDevice, logicalDevice,
			vertices, vertexBuffer, vertexBufferMemory, graphicsQueue, commandPool);

		rollback->Add([this]()
		{
			spdlog::info("Dispose Vertex Buffer");
			VkUtils::DisposeVertexBuffer(logicalDevice, vertexBuffer, vertexBufferMemory);
		});
	}

	void MeshVulkan::CreateIndexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<Rollback> rollback)
	{
		VkUtils::CreateIndexBuffer(physicalDevice, logicalDevice,
			indices, indexBuffer, indexBufferMemory, graphicsQueue, commandPool);

		rollback->Add([this]()
		{
			spdlog::info("Dispose Index Buffer");
			VkUtils::DisposeIndexBuffer(logicalDevice, indexBuffer, indexBufferMemory);
		});
	}
}