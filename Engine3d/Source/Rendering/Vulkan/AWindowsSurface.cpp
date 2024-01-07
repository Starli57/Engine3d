#include "Pch.h"
#include "AWindowsSurface.h"

namespace AVulkan
{
	VkSurfaceKHR AWindowsSurface::Create(VkInstance& instance, GLFWwindow& window) const
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
		if (createResult != VK_SUCCESS) throw std::runtime_error("Cant't create vulkan window surface, status: " + createResult);
		return surface;
	}

	void AWindowsSurface::Dispose(VkInstance& instance, VkSurfaceKHR& surface) const
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
}