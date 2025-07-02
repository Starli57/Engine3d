#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VkUtils
{
	VkVertexInputBindingDescription GetVertexInputBindingDescription();
	std::array<VkVertexInputAttributeDescription, 6> GetVertexInputAttributeDescriptions();

	void CreateVertexBuffer(const Ref<AVulkan::VulkanContext>& context, std::vector<Vertex>& vertices,
		VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool);

	void DisposeVertexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory);
}
