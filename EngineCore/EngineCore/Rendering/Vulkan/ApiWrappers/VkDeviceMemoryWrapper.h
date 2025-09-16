#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
	uint32_t FindMemoryType(const VkPhysicalDevice& physicalDevice, const uint32_t& typeFilter, const VkMemoryPropertyFlags& properties);
	void FreeDeviceMemory(const VkDevice& logicalDevice, const VkDeviceMemory& memory);
}
