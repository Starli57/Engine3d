#include "Pch.h"
#include "WindowsSurfaceInterface.h"

VkSurfaceKHR WindowsSurfaceInterface::CreateSurface(VkInstance& instance, GLFWwindow& window) const
{
	spdlog::info("Create Window Surface");

	VkSurfaceKHR surface;
	auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
	if (createResult != VK_SUCCESS) throw std::runtime_error("Cant't create vulkan window surface, status: " + createResult);
	return surface;
}

void WindowsSurfaceInterface::DestroySurface(VkInstance& instance, VkSurfaceKHR& surface) const
{
	spdlog::info("Dispose surface");
	vkDestroySurfaceKHR(instance, surface, nullptr);
}