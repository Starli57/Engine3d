#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(VkInstance& instance, GLFWwindow& window);
	void DisposeSurface(VkInstance& instance, VkSurfaceKHR& surface);
}