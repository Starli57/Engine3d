#include "EngineCore/Pch.h"
#include "SwapChain.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "Utilities/ImageUtility.h"
#include "Utilities/SwapchainUtility.h"

namespace AVulkan
{
	SwapChain::SwapChain(const Ref<VulkanContext>& context, Ref<SwapChainData> swapChainData) :
		vulkanContext(context), swapChainData(swapChainData)
	{
		physicalDeviceQueueIndices = VkUtils::GetQueueFamilies(context->physicalDevice, context->windowSurface);
	}

	SwapChain::~SwapChain()
	{
	}

	void SwapChain::Recreate()
	{
		Dispose();

		CreateSwapchain();
		CreateSwapChainImageViews();
		CreateDepthBuffer();
		CreateMSAAColorResources();
		CreateMSAADepthResources();
	}

	void SwapChain::CreateSwapchain()
	{
		spdlog::info("Create swap chain");
		
		auto details = VkUtils::GetSwapChainDetails(vulkanContext->physicalDevice, vulkanContext->windowSurface);
		CAssert::Check(VkUtils::DoSupportSwapChain(details), "Swap chains are not supported");

		swapChainData->surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(*vulkanContext->window, details.capabilities);

		swapChainData->imagesCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && swapChainData->imagesCount > details.capabilities.maxImageCount)
		{
			swapChainData->imagesCount = details.capabilities.maxImageCount;
		}

		spdlog::info("Swap chain images count: {0}", swapChainData->imagesCount);


		VkSwapchainCreateInfoKHR createInfo{};
		SetupSwapChainInfo(createInfo, vulkanContext->windowSurface, extent, presentMode, swapChainData->surfaceFormat, 
			details.capabilities, physicalDeviceQueueIndices, swapChainData->imagesCount);

		auto createStatus = vkCreateSwapchainKHR(vulkanContext->logicalDevice, &createInfo, nullptr, &swapChainData->swapChain);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create swap chain, status: " + createStatus);

		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapChainData->swapChain, &swapChainData->imagesCount, nullptr);
		swapChainData->images.resize(swapChainData->imagesCount);
		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapChainData->swapChain, &swapChainData->imagesCount, swapChainData->images.data());

		vulkanContext->imageFormat = swapChainData->surfaceFormat.format;
		swapChainData->extent = extent;
	}

	void SwapChain::CreateSwapChainImageViews() const
	{
		swapChainData->imageViews.resize(swapChainData->images.size());
		for (int i = 0; i < swapChainData->imageViews.size(); i++)
		{
			VkUtils::CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
				swapChainData->images[i], swapChainData->imageViews[i]);
		}
	}

	void SwapChain::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");

		swapChainData->depthBufferModel = CreateRef<ImageModel>();

		VkUtils::CreateImage(
			vulkanContext, swapChainData->extent.width, swapChainData->extent.height, 1, vulkanContext->depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_SAMPLE_COUNT_1_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapChainData->depthBufferModel->image,
			swapChainData->depthBufferModel->imageMemory);

		VkUtils::CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			swapChainData->depthBufferModel->image, swapChainData->depthBufferModel->imageView);
	}

	void SwapChain::CreateMSAAColorResources() const
	{
		spdlog::info("Create msaa color buffer");
		swapChainData->msaaColorSample = CreateRef<ImageModel>();
		VkUtils::CreateImage(vulkanContext, 
			swapChainData->extent.width, swapChainData->extent.height, 1, vulkanContext->imageFormat,
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			vulkanContext->msaa, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapChainData->msaaColorSample->image,
			swapChainData->msaaColorSample->imageMemory);

		VkUtils::CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
			swapChainData->msaaColorSample->image, swapChainData->msaaColorSample->imageView);
	}

	void SwapChain::CreateMSAADepthResources() const
	{
		spdlog::info("Create msaa depth buffer");
		
		swapChainData->msaaDepthSample = CreateRef<ImageModel>();
		VkUtils::CreateImage(vulkanContext, 
			swapChainData->extent.width, swapChainData->extent.height, 1, vulkanContext->depthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			vulkanContext->msaa, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapChainData->msaaDepthSample->image,
			swapChainData->msaaDepthSample->imageMemory);

		VkUtils::CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			swapChainData->msaaDepthSample->image, swapChainData->msaaDepthSample->imageView);
	}

	void SwapChain::Dispose() const
	{
		spdlog::info("Dispose swapchain");

		VkUtils::DisposeImageModel(vulkanContext->logicalDevice, swapChainData->msaaDepthSample);
		VkUtils::DisposeImageModel(vulkanContext->logicalDevice, swapChainData->msaaColorSample);
		VkUtils::DisposeImageModel(vulkanContext->logicalDevice, swapChainData->depthBufferModel);

		for (int i = 0; i < swapChainData->imageViews.size(); i++)
			VkUtils::DestroyImageView(vulkanContext->logicalDevice, swapChainData->imageViews.at(i));
		swapChainData->imageViews.clear();

		vkDestroySwapchainKHR(vulkanContext->logicalDevice, swapChainData->swapChain, nullptr);
	}
	
	void SwapChain::SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
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

			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
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

	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
	{
		//todo: chose format by player settings or most often used

		for (const auto& data : availableFormats)
		{
			if (data.format == VK_FORMAT_R8G8B8A8_SRGB && data.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				spdlog::info("Chosen color format: {0}", static_cast<int>(data.format));
				return data;
			}
		}

		spdlog::warn("Fallback color format: {0}", static_cast<int>(availableFormats[0].format));
		return availableFormats[0];
	}

	VkExtent2D SwapChain::ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const
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

	VkPresentModeKHR SwapChain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const
	{
		constexpr VkPresentModeKHR highQualityMode = VK_PRESENT_MODE_MAILBOX_KHR;
		constexpr VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& mode : availableModes)
		{
			if (mode == highQualityMode)
			{
				return mode;
			}
		}

		spdlog::warn("Present mode is not available: {0} Default mode will be used {1}", static_cast<int>(highQualityMode), static_cast<int>(defaultMode));
		return defaultMode;
	}

}
