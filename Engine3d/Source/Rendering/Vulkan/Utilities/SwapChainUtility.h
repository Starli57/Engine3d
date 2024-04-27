#pragma once

#include <GLFW/glfw3.h>
#include "Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"

namespace AVulkan
{
	class SwapChainUtility
	{
	public:

		SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;

		bool DoSupportSwapChain(SwapChainSurfaceSettings& details) const;
		bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;

		void GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats) const;
		void GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes) const;
	};
}