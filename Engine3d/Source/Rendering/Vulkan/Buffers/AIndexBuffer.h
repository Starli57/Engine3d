#pragma once

#include <GLFW/glfw3.h>
#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/VulkanModel.h"

namespace AVulkan
{
	class AIndexBuffer
	{
	public:
		void Create(Ref<VulkanModel> vulkanModel, Ref<std::vector<uint32_t>> indices,
			VkBuffer& indexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool) const;
		void Dispose(VkDevice& logicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	};
}
