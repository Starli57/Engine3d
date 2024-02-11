#pragma once

#include <GLFW/glfw3.h>

#include "ABuffer.h"
#include "Rendering/Model/UboViewProjection.h"

namespace AVulkan
{
	class UniformBufferVulkan
	{
	public:
		UniformBufferVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkDeviceSize bufferSize);
		~UniformBufferVulkan();

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;

	private:
		VkDevice* logicalDevice;
	};
}
