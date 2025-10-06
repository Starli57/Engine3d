#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

	bool DoSupportSwapChain(const SwapChainSurfaceSettings& details);
	bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

	void GetSwapChainColorFormats(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats);
	void GetSwapChainPresentModes(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes);

}
