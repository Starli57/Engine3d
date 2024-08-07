#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();

		void Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<std::vector<Vertex>> vertices,
			VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool) const;

		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;


	};
}
