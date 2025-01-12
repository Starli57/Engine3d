#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

namespace VkUtils
{
	VkVertexInputBindingDescription GetVertexInputBindingDescription();
	std::array<VkVertexInputAttributeDescription, 6> GetVertexInputAttributeDescriptions();

	void CreateVertexBuffer(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, std::vector<Vertex>& vertices,
		VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkQueue& graphicsQueue, VkCommandPool& commandPool);

	void DisposeVertexBuffer(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
}
