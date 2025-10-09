#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
	void GetSwapChainColorFormats(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats);
	void GetSwapChainPresentModes(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes);

}
