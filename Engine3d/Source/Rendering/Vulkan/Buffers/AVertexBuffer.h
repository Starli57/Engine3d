#pragma once

#include <GLFW/glfw3.h>

#include "SharedLib/Ref.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/VulkanModel.h"

namespace AVulkan
{
	class AVertexBuffer
	{
	public:
		VkVertexInputBindingDescription GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

		void Create(Ref<VulkanModel> vulkanModel, Ref<std::vector<Vertex>> vertices, VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory) const;
		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	};
}
