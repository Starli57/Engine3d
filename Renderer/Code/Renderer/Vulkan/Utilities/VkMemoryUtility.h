#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class VkMemoryUtility
	{
	public:
		static uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties);
		static void FreeDeviceMemory(VkDevice& logicalDevice, VkDeviceMemory& memory);
	};
}
