#include "Pch.h"
#include "ASwapChain.h"

namespace AVulkan
{
	//todo: div to smaller functions
	SwapChainData ASwapChain::Create(GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
		VkSurfaceKHR& surface, QueueFamilyIndices& physicalDeviceQueueIndices) const
	{
		spdlog::info("Create swap chain");

		SwapChainData swapChainData;

		auto details = GetSwapChainDetails(physicalDevice, surface);
		if (!DoSupportSwapChain(details)) throw std::runtime_error("Swap chains are not supported");

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(window, details.capabilities);

		//todo: setup how much exactly images do we need
		uint32_t imageCount = details.capabilities.minImageCount + 1;

		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
		{
			imageCount = details.capabilities.maxImageCount;
		}

		spdlog::info("Swap chain images count: {0}", imageCount);


		VkSwapchainCreateInfoKHR createInfo{};
		SetupSwapChainInfo(createInfo, surface, extent, presentMode, surfaceFormat, details.capabilities, physicalDeviceQueueIndices, imageCount);

		auto createStatus = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChainData.swapChain);
		if (createStatus != VK_SUCCESS) throw std::runtime_error("Failed to create swap chain, status: " + createStatus);

		vkGetSwapchainImagesKHR(logicalDevice, swapChainData.swapChain, &imageCount, nullptr);
		swapChainData.images.resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapChainData.swapChain, &imageCount, swapChainData.images.data());

		swapChainData.imageFormat = surfaceFormat.format;
		swapChainData.extent = extent;

		return swapChainData;
	}

	void ASwapChain::Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const
	{
		spdlog::info("Dispose swap chain");
		vkDestroySwapchainKHR(logicalDevice, swapChainData.swapChain, nullptr);
	}
	
	void ASwapChain::SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
		VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities, 
		QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const
	{
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
		createInfo.preTransform = capabilities.currentTransform;
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
	}

	SwapChainSurfaceSettings ASwapChain::GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const
	{
		SwapChainSurfaceSettings surfaceSettigns;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceSettigns.capabilities);

		GetSwapChainColorFormats(physicalDevice, surface, surfaceSettigns.formats);
		GetSwapChainPresentModes(physicalDevice, surface, surfaceSettigns.presentModes);

		return surfaceSettigns;
	}

	void ASwapChain::GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats) const
	{
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	}

	void ASwapChain::GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes) const
	{
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	}

	VkSurfaceFormatKHR ASwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
	{
		//todo: chose format by player settings or most often used

		for (const auto& data : availableFormats)
		{
			if (data.format == VK_FORMAT_R8G8B8A8_SRGB && data.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				spdlog::info("Choosed color format: {0}", data.format);
				return data;
			}
		}

		spdlog::warn("fallback color format: {0}", availableFormats[0].format);
		return availableFormats[0];
	}

	VkExtent2D ASwapChain::ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		int width, height;
		glfwGetFramebufferSize(&window, &width, &height);

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	VkPresentModeKHR ASwapChain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const
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

		spdlog::warn("Present mode is not available: {0} Fallback mode will be used {1}", bestMode, fallback);
		return fallback;
	}

	bool ASwapChain::DoSupportSwapChain(SwapChainSurfaceSettings& details) const
	{
		return !details.formats.empty() && !details.presentModes.empty();
	}

	bool ASwapChain::DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const
	{
		SwapChainSurfaceSettings details = GetSwapChainDetails(physicalDevice, surface);
		return DoSupportSwapChain(details);
	}
}