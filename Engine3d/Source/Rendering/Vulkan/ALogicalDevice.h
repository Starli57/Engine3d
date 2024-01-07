#pragma once

#include <GLFW/glfw3.h>

class ALogicalDevice
{
public:
	VkDevice Create(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface,
		VkQueue& graphicsQueue, VkQueue& presentationQueue) const;

	void Dispose(VkDevice& logicalDevice) const;
};

