#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"

namespace VkExtensions
{
	AVulkan::SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

	bool DoSupportSwapChain(AVulkan::SwapChainSurfaceSettings& details);
	bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

	void GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats);
	void GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes);

}