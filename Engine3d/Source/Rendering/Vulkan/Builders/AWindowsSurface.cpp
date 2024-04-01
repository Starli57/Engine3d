#include "Pch.h"
#include "AWindowsSurface.h"

namespace AVulkan
{
	VkSurfaceKHR AWindowsSurface::Create(Ref<VulkanModel> vulkanModel) const
	{
		spdlog::info("Create Window Surface");

		VkSurfaceKHR surface;
		auto createResult = glfwCreateWindowSurface(vulkanModel->instance, vulkanModel->window, nullptr, &surface);
		CAssert::Check(createResult == VK_SUCCESS, "Cant't create vulkan window surface, status: " + createResult);

		return surface;
	}

	void AWindowsSurface::Dispose(Ref<VulkanModel> vulkanModel) const
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(vulkanModel->instance, vulkanModel->windowSurface, nullptr);
	}
}