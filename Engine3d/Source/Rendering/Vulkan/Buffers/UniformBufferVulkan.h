#pragma once

#include <GLFW/glfw3.h>

#include "ABuffer.h"
#include "Rendering/Model/Mvp.h"

namespace AVulkan
{
	class UniformBufferVulkan
	{
	public:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;

		UniformBufferVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice);
		~UniformBufferVulkan();


	private:
		VkDevice* logicalDevice;
	};
}
