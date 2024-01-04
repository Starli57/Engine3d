#include "Pch.h"
#include "SwapChainInterface.h"

void SwapChainInterface::CreateSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	auto details = GetSwapChainDetails(device, surface);
	if (!DoSupportSwapChain(details))
	{
		std::cout << "This Device doesn't support swap chains" << std::endl;//todo: add better error handling
		return;
	}

}

SwapChainDetails SwapChainInterface::GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	GetSwapChainColorFormats(device, surface, details.formats);
	GetSwapChainPresentModes(device, surface, details.presentModes);

	return details;
}

void SwapChainInterface::GetSwapChainColorFormats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats)
{
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
}

void SwapChainInterface::GetSwapChainPresentModes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& presentModes)
{
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());

}

VkSurfaceFormatKHR SwapChainInterface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	//todo: chose format by player settings or most often used

	for (const auto& data : availableFormats) 
	{
		if (data.format == VK_FORMAT_R8G8B8A8_SRGB && data.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			std::cout << "Choosed color format: " << data.format << std::endl;
			return data;
		}
	}

	std::cout << "fallback color format: " << availableFormats[0].format << std::endl;
	return availableFormats[0];
}

VkPresentModeKHR SwapChainInterface::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_MAILBOX_KHR;
	VkPresentModeKHR fallback = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& mode : availableModes)
	{
		if (mode == bestMode)
		{
			return mode;
		}
	}

	std::cout << "Present mode is not available: " << bestMode << std::endl;

	return fallback;
}

bool SwapChainInterface::DoSupportSwapChain(SwapChainDetails details)
{
	return !details.formats.empty() && !details.presentModes.empty();
}

bool SwapChainInterface::DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainDetails details = GetSwapChainDetails(device, surface);
	return DoSupportSwapChain(details);
}
