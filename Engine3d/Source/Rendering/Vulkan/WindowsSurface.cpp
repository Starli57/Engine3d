#include "Pch.h"
#include "WindowsSurface.h"

VkSurfaceKHR WindowsSurface::CreateSurface(VkInstance instance, GLFWwindow& window)
{
	VkSurfaceKHR surface;
	auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
	if (createResult != VK_SUCCESS) std::cout << "Cant't create vulkan window surface, status: " << createResult << std::endl;
	return surface;
}
