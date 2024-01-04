#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class LogicalDeviceInterface
{
public:
	VkDevice Create(VkPhysicalDevice physicalDevice, VkSurfaceKHR& windowSurface,
		VkQueue& graphicsQueue, VkQueue& presentationQueue);
};

