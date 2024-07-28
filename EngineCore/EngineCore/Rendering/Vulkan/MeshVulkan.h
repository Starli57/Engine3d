#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <filesystem>

#include "EngineCore/Mesh.h"
#include "EngineCore/Vertex.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"

namespace AVulkan
{
	class MeshVulkan : public Mesh
	{
	public:
		VkBuffer GetVertexBuffer();
		VkBuffer GetIndexBuffer();

		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, 
			VkCommandPool& commandPool, const std::filesystem::path& path, Ref<Rollback> rollback);

		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, 
			VkCommandPool& commandPool, Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, Ref<Rollback> rollback);

		virtual ~MeshVulkan() override;

	private:
		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		void CreateVertexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<Rollback> rollback);
		void CreateIndexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<Rollback> rollback);
	};
}
