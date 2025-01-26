#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(const VkInstance& instance, GLFWwindow& window);
	void DisposeSurface(const VkInstance& instance, const VkSurfaceKHR& surface);
}