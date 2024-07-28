#include "EngineCore/Pch.h"
#include "WindowSurfaceUtility.h"

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(VkInstance& instance, GLFWwindow& window)
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		auto createResult = glfwCreateWindowSurface(instance, &window, nullptr, &surface);
		CAssert::Check(createResult == VK_SUCCESS, "Cant't create vulkan window surface, status: " + createResult);

		return surface;
	}

	void DisposeSurface(VkInstance& instance, VkSurfaceKHR& surface)
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
}