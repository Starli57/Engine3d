#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Buffers/Vertex.h"

namespace AVulkan
{
	class Mesh
	{
	public:
		Mesh(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices);
		~Mesh();

		uint32_t GetVertexCount();
		VkBuffer GetVertexBuffer();
	private:
		VkDevice* logicalDevice;
		std::vector<Vertex>* vertices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}
