#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class WindowsSurface
{
public:
	VkSurfaceKHR CreateSurface(VkInstance instance, GLFWwindow& window);
};

