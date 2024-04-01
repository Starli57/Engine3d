#include "Pch.h"
#include "SwapChainUtility.h"


namespace AVulkan
{
	SwapChainSurfaceSettings SwapChainUtility::GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const
	{
		SwapChainSurfaceSettings surfaceSettigns;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceSettigns.capabilities);

		GetSwapChainColorFormats(physicalDevice, surface, surfaceSettigns.formats);
		GetSwapChainPresentModes(physicalDevice, surface, surfaceSettigns.presentModes);

		return surfaceSettigns;
	}

	bool SwapChainUtility::DoSupportSwapChain(SwapChainSurfaceSettings& details) const
	{
		return !details.formats.empty() && !details.presentModes.empty();
	}

	bool SwapChainUtility::DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const
	{
		SwapChainSurfaceSettings details = GetSwapChainDetails(physicalDevice, surface);
		return DoSupportSwapChain(details);
	}

	void SwapChainUtility::GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats) const
	{
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	}

	void SwapChainUtility::GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes) const
	{
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	}
}