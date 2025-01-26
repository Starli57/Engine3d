#include "EngineCore/Pch.h"
#include "WindowSurfaceUtility.h"

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(const VkInstance& instance, GLFWwindow& window)
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		const auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
		CAssert::Check(createResult == VK_SUCCESS, "Can't create vulkan window surface, status: " + createResult);

		return surface;
	}

	void DisposeSurface(const VkInstance& instance, const VkSurfaceKHR& surface)
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
}