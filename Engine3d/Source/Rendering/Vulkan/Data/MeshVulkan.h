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
	private:
		VkDevice* logicalDevice;
		std::vector<Vertex> vertices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
	};
}
