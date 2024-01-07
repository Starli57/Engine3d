#pragma once

#include <GLFW/glfw3.h>

class LogicalDeviceInterface
{
public:
	VkDevice Create(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface,
		VkQueue& graphicsQueue, VkQueue& presentationQueue) const;

	void DestroyDevice(VkDevice& logicalDevice) const;
};

