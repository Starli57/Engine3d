#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class LogicalDeviceCreator
{
public:
	VkDevice Create(VkPhysicalDevice device);
};

