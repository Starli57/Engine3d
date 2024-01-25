#pragma once

#include <GLFW/glfw3.h>
#include "Vertex.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		void Create(VkDevice& logicalDevice, std::vector<Vertex>& vertices, VkBufferCreateInfo& bufferInfo, VkBuffer& vertexBuffer) const;
		void Setup(std::vector<Vertex>& vertices, VkBufferCreateInfo& bufferInfo) const;

		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

		void Dispose(VkDevice& logicalDevice, VkBuffer& vertexBuffer) const;
	};
}
