#include "Pch.h"
#include "VkSwapchainUtility.h"

namespace VkUtilities
{
	AVulkan::SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
	{
		AVulkan::SwapChainSurfaceSettings surfaceSettigns;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceSettigns.capabilities);

		GetSwapChainColorFormats(physicalDevice, surface, surfaceSettigns.formats);
		GetSwapChainPresentModes(physicalDevice, surface, surfaceSettigns.presentModes);

		return surfaceSettigns;
	}

	bool DoSupportSwapChain(AVulkan::SwapChainSurfaceSettings& details)
	{
		return !details.formats.empty() && !details.presentModes.empty();
	}

	bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
	{
		AVulkan::SwapChainSurfaceSettings details = GetSwapChainDetails(physicalDevice, surface);
		return DoSupportSwapChain(details);
	}

	void GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats)
	{
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	}

	void GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes)
	{
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	}
}