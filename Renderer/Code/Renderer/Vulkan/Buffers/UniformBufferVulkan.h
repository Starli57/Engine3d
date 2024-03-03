#pragma once

#include <GLFW/glfw3.h>

#include "ABuffer.h"
#include "Renderer/Model/UboViewProjection.h"

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
