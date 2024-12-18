#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <filesystem>

#include "EngineCore/Assets/Mesh.h"
#include "EngineCore/Rendering/Vertex.h"
#include "EngineCore/Core/Ref.h"
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
			VkCommandPool& commandPool, const std::filesystem::path& path);

		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, 
			VkCommandPool& commandPool, Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);

		virtual ~MeshVulkan() override;

	private:
		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		void CreateVertexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool);
		void CreateIndexBuffer(VkQueue& graphicsQueue, VkCommandPool& commandPool);
	};
}
