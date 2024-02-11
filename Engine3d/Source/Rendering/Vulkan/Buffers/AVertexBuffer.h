#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Model/Vertex.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

		void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices,
			VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const;

		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;


	};
}
