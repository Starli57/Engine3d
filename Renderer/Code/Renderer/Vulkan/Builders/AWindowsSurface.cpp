#include "Pch.h"
#include "AWindowsSurface.h"

namespace AVulkan
{
	VkSurfaceKHR AWindowsSurface::Create(VkInstance& instance, GLFWwindow& window) const
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
		CAssert::Check(createResult == VK_SUCCESS, "Cant't create vulkan window surface, status: " + createResult);

		return surface;
	}

	void AWindowsSurface::Dispose(VkInstance& instance, VkSurfaceKHR& surface) const
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
}