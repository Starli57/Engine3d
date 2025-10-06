#include "EngineCore/Pch.h"
#include "VkSurfaceKHRWrapper.h"

#include "EngineCore/CustomAssert.h"

namespace VulkanApi
{
	void CreateSurface(VulkanContext* vulkanContext, GLFWwindow* window)
	{
		spdlog::info("Create Window Surface");

		const auto createResult = glfwCreateWindowSurface(vulkanContext->instance, window, nullptr, &vulkanContext->windowSurface);
		Engine::CAssert::Check(createResult == VK_SUCCESS, "Can't create vulkan window surface, status: " + createResult);
	}

	void DisposeSurface(VulkanContext* vulkanContext)
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(vulkanContext->instance, vulkanContext->windowSurface, nullptr);
	}
}
