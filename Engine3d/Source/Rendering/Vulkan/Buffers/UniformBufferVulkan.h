#pragma once

#include <GLFW/glfw3.h>

#include "ABuffer.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Rendering/Vulkan/VulkanModel.h"

namespace AVulkan
{
	class UniformBufferVulkan
	{
	public:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;

		UniformBufferVulkan(Ref<VulkanModel> vulkanModel);
		~UniformBufferVulkan();


	private:
		VkDevice* logicalDevice;
	};
}
