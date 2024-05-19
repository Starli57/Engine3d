#pragma once

#include <GLFW/glfw3.h>

namespace VkMemoryExtension
{
	uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties);
	void FreeDeviceMemory(VkDevice& logicalDevice, VkDeviceMemory& memory);
}
