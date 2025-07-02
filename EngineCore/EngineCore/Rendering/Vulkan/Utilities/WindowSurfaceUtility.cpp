#include "EngineCore/Pch.h"
#include "WindowSurfaceUtility.h"

#include <pybind11/detail/common.h>

#include "EngineCore/CustomAssert.h"

namespace VkUtils
{
	VkSurfaceKHR CreateSurface(const Ref<AVulkan::VulkanContext>& vulkanContext)
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		const auto createResult = glfwCreateWindowSurface(vulkanContext->instance, vulkanContext->window, nullptr, &surface);
		CAssert::Check(createResult == VK_SUCCESS, "Can't create vulkan window surface, status: " + createResult);

		return surface;
	}

	void DisposeSurface(const Ref<AVulkan::VulkanContext>& vulkanContext)
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(vulkanContext->instance, vulkanContext->windowSurface, nullptr);
	}
}
