#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkCommandPool CreateCommandPool(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface);
	void DisposeCommandPool(VkDevice& logicalDevice, VkCommandPool& commandPool);
}