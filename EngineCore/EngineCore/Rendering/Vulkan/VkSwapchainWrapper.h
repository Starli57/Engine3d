#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "Models/SwapchainSurfaceSettings.h"

namespace VulkanApi
{
	void RecreateSwapchain(VulkanContext* vulkanContext);
	void CreateSwapchain(VulkanContext* vulkanContext);
	void CreateSwapchainImageViews(VulkanContext* vulkanContext);
	void CreateDepthBuffer(VulkanContext* vulkanContext);
	void CreateMSAAColorResources(VulkanContext* vulkanContext);
	void CreateMSAADepthResources(VulkanContext* vulkanContext);
	void DisposeSwapchain(const VulkanContext* vulkanContext);

	void SetPresentMode(const std::vector<VkPresentModeKHR>& availableModes, SwapchainContext* swapchainContext);
	void SetSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, SwapchainContext* swapchainContext);
	void SetSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanContext* vulkanContext);

	void SetSwapchainInfo(VulkanContext* vulkanContext, VkSurfaceCapabilitiesKHR& capabilities, VkSwapchainCreateInfoKHR& outCreateInfo);

	void SetSwapchainDetails(const VulkanContext* vulkanContext, SwapchainSurfaceSettings& surfaceSettings);
	void SetSwapchainColorFormats(const VulkanContext* vulkanContext, std::vector<VkSurfaceFormatKHR>& outFormats);
	void SetSwapchainPresentModes(const VulkanContext* vulkanContext, std::vector<VkPresentModeKHR>& outPresentModes);
	
	bool DoSupportSwapchain(const SwapchainSurfaceSettings& details);
	bool DoSupportSwapchain(const VulkanContext* vulkanContext);
}
