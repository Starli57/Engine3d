#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkCommandPool CreateCommandPool(const VkDevice& logicalDevice, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& windowSurface);
	void DisposeCommandPool(const VkDevice& logicalDevice, const VkCommandPool& commandPool);
}