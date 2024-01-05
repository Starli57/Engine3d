#include "Pch.h"
#include "SwapChainInterface.h"

//todo: div to smaller functions
VkSwapchainKHR SwapChainInterface::CreateSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, 
	VkSurfaceKHR surface, QueueFamilyIndices& physicalDeviceQueueIndices)
{
	auto details = GetSwapChainDetails(physicalDevice, surface);
	if (!DoSupportSwapChain(details))
	{
		throw std::runtime_error("Swap chains are not supported");//todo: make better error handling
	}

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
	VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
	VkExtent2D extent = ChooseSwapExtent(window, details.capabilities);

	//todo: setup how much exactly images do we need
	uint32_t imageCount = details.capabilities.minImageCount + 1;

	if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) 
	{
		imageCount = details.capabilities.maxImageCount;
	}

	std::cout << "Swap chain images count: " << imageCount << std::endl;


	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.imageExtent = extent;
	createInfo.presentMode = presentMode;
	createInfo.minImageCount = imageCount;

	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;

	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.preTransform = details.capabilities.currentTransform;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (physicalDeviceQueueIndices.graphicsFamily != physicalDeviceQueueIndices.presentationFamily) 
	{
		uint32_t queueFamilyIndices[] = { physicalDeviceQueueIndices.graphicsFamily.value(), physicalDeviceQueueIndices.presentationFamily.value() };

		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; 
		createInfo.pQueueFamilyIndices = nullptr;
	}

	VkSwapchainKHR swapChain;
	auto createStatus = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain);
	if (createStatus != VK_SUCCESS)
	{
		//todo: make better errors handling
		throw std::runtime_error("Failed to create swap chain, status: " + createStatus);
	}

	return swapChain;
}

SwapChainDetails SwapChainInterface::GetSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	SwapChainDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

	GetSwapChainColorFormats(physicalDevice, surface, details.formats);
	GetSwapChainPresentModes(physicalDevice, surface, details.presentModes);

	return details;
}

void SwapChainInterface::GetSwapChainColorFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats)
{
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
}

void SwapChainInterface::GetSwapChainPresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& presentModes)
{
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

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

VkExtent2D SwapChainInterface::ChooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	VkExtent2D actualExtent = 
	{
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};

	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
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

	std::cout << "Present mode is not available: " << bestMode << " Fallback mode will be used" << std::endl;

	return fallback;
}

bool SwapChainInterface::DoSupportSwapChain(SwapChainDetails details)
{
	return !details.formats.empty() && !details.presentModes.empty();
}

bool SwapChainInterface::DoSupportSwapChain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	SwapChainDetails details = GetSwapChainDetails(physicalDevice, surface);
	return DoSupportSwapChain(details);
}
