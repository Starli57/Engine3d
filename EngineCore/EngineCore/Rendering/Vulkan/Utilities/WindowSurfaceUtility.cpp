#include "EngineCore/Pch.h"
#include "WindowSurfaceUtility.h"

#include "EngineCore/CustomAssert.h"

namespace VulkanApi
{
	void CreateSurface(const Ref<VulkanContext>& vulkanContext)
	{
		spdlog::info("Create Window Surface");

		const auto createResult = glfwCreateWindowSurface(vulkanContext->instance, vulkanContext->window, nullptr, &vulkanContext->windowSurface);
		Engine::CAssert::Check(createResult == VK_SUCCESS, "Can't create vulkan window surface, status: " + createResult);
	}

	void DisposeSurface(const Ref<VulkanContext>& vulkanContext)
	{
		spdlog::info("Dispose surface");
		vkDestroySurfaceKHR(vulkanContext->instance, vulkanContext->windowSurface, nullptr);
	}
}
