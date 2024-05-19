#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkDevice CreateLogicalDevice(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface, VkQueue& graphicsQueue, VkQueue& presentationQueue);
	void DisposeLogicalDevice(VkDevice& logicalDevice);
}