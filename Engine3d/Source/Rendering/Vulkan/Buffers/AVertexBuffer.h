#pragma once

#include <GLFW/glfw3.h>

#include "Architecture/Ref.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/VulkanContext.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

		void Create(Ref<VulkanContext> vulkanContext, Ref<std::vector<Vertex>> vertices,
			VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory) const;

		void Dispose(Ref<VulkanContext> vulkanContext, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;


	};
}
