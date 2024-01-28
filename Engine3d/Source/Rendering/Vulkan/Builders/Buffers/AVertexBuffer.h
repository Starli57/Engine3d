#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Data/Vertex.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		void Create(VkPhysicalDevice physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices,
			VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	};
}
