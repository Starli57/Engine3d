#pragma once

#include <GLFW/glfw3.h>

#include "ABuffer.h"
#include "Rendering/Data/Mvp.h"

namespace AVulkan
{
	class UniformBufferVulkan
	{
	public:
		UniformBufferVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice);
		~UniformBufferVulkan();

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;

	private:
		VkDevice* logicalDevice;
	};
}
