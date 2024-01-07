#pragma once

#include <GLFW/glfw3.h>

class WindowsSurfaceInterface
{
public:
	VkSurfaceKHR CreateSurface(VkInstance& instance, GLFWwindow& window) const;
	void DestroySurface(VkInstance& instance, VkSurfaceKHR& surface) const;
};

