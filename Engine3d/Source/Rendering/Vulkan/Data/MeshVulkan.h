#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Rendering/Data/Mesh.h"
#include "Rendering/Data/Vertex.h"

namespace AVulkan
{
	class MeshVulkan
	{
	public:
		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, Mesh& mesh);
		~MeshVulkan();

		uint32_t GetVertexCount();
		VkBuffer GetVertexBuffer();

		uint32_t GetIndicesCount();
		VkBuffer GetIndexBuffer();

	private:
		VkDevice* logicalDevice;
		std::vector<Vertex>* vertices;
		std::vector<uint32_t>* indices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
	};
}
