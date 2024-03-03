#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "SharedLib/Ref.h"
#include "Renderer/Model/Vertex.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

		void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<std::vector<Vertex>> vertices,
			VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const;

		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;


	};
}
