#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "VulkanContext.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapchainContext.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"

namespace VulkanApi
{
	class SwapchainManager
	{
	public:

		SwapchainManager(VulkanContext* vulkanContext);
		~SwapchainManager();

		void Recreate();
		void CreateSwapchain();
		void CreateSwapChainImageViews() const;
		void CreateDepthBuffer() const;
		void CreateMSAAColorResources() const;
		void CreateMSAADepthResources() const;
		void Dispose() const;

	private:
		VulkanContext* vulkanContext;
		
		QueueFamilyIndices physicalDeviceQueueIndices;

		SwapchainContext* GetSwapchainContext() const { return vulkanContext->swapchainContext; }
		
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

		void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
			VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
			QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const;
	};
}
