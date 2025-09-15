#include "EngineCore/Pch.h"
#include "SwapchainUtility.h"

namespace VulkanApi
{
	AVulkan::SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
	{
		AVulkan::SwapChainSurfaceSettings surfaceSettings;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceSettings.capabilities);

		GetSwapChainColorFormats(physicalDevice, surface, surfaceSettings.formats);
		GetSwapChainPresentModes(physicalDevice, surface, surfaceSettings.presentModes);

		return surfaceSettings;
	}

	bool DoSupportSwapChain(const AVulkan::SwapChainSurfaceSettings& details)
	{
		return !details.formats.empty() && !details.presentModes.empty();
	}

	bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
	{
		AVulkan::SwapChainSurfaceSettings details = GetSwapChainDetails(physicalDevice, surface);
		return DoSupportSwapChain(details);
	}

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