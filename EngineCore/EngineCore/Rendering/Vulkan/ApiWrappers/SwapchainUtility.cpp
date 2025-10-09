#include "EngineCore/Pch.h"
#include "SwapchainUtility.h"

namespace VulkanApi
{
	void GetSwapChainColorFormats(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats)
	{
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	}

	void GetSwapChainPresentModes(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes)
	{
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	}
}