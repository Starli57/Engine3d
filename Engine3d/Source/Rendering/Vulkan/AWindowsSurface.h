#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class AWindowsSurface
	{
	public:
		VkSurfaceKHR Create(VkInstance& instance, GLFWwindow& window) const;
		void Dispose(VkInstance& instance, VkSurfaceKHR& surface) const;
	};
}