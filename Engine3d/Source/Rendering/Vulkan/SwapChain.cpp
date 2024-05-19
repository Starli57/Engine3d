#include "Pch.h"
#include "SwapChain.h"

namespace AVulkan
{
	SwapChain::SwapChain(Ref<Rollback> rollback, GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkSurfaceKHR& surface,
		QueueFamilyIndices& physicalDeviceQueueIndices, VkQueue& graphicsQueue, Ref<SwapChainData> swapChainData) :
		window(window), physicalDevice(physicalDevice), logicalDevice(logicalDevice), surface(surface),
		physicalDeviceQueueIndices(physicalDeviceQueueIndices), graphicsQueue(graphicsQueue), swapChainData(swapChainData)
	{
		this->rollback = CreateRef<Rollback>("SwapChain", rollback);
	}

	SwapChain::~SwapChain()
	{
		Dispose();
	}

	void SwapChain::Recreate()
	{
		rollback->Dispose();

		CreateSwapchain();
		CreateSwapChainImageViews();
		CreateDepthBuffer(commandPool);
		CreateFrameBuffers(renderPass);
	}

	void SwapChain::CreateSwapchain()
	{
		spdlog::info("Create swap chain");
		
		auto details = SwapchainExtension::GetSwapChainDetails(physicalDevice, surface);
		CAssert::Check(SwapchainExtension::DoSupportSwapChain(details), "Swap chains are not supported");

		swapChainData->surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(window, details.capabilities);

		swapChainData->imagesCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && swapChainData->imagesCount > details.capabilities.maxImageCount)
		{
			swapChainData->imagesCount = details.capabilities.maxImageCount;
		}

		spdlog::info("Swap chain images count: {0}", swapChainData->imagesCount);


		VkSwapchainCreateInfoKHR createInfo{};
		SetupSwapChainInfo(createInfo, surface, extent, presentMode, swapChainData->surfaceFormat, 
			details.capabilities, physicalDeviceQueueIndices, swapChainData->imagesCount);

		auto createStatus = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChainData->swapChain);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create swap chain, status: " + createStatus);

		vkGetSwapchainImagesKHR(logicalDevice, swapChainData->swapChain, &swapChainData->imagesCount, nullptr);
		swapChainData->images.resize(swapChainData->imagesCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapChainData->swapChain, &swapChainData->imagesCount, swapChainData->images.data());

		swapChainData->imageFormat = swapChainData->surfaceFormat.format;
		swapChainData->extent = extent;


		rollback->Add([this]() { vkDestroySwapchainKHR(logicalDevice, swapChainData->swapChain, nullptr); });
	}

	void SwapChain::CreateSwapChainImageViews()
	{
		swapChainData->imageViews.reserve(swapChainData->images.size());
		for (int i = 0; i < swapChainData->images.size(); i++)
		{
			AImageView().Create(logicalDevice, swapChainData->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
				swapChainData->images[i], swapChainData->imageViews[i], rollback);
		}
	}

	void SwapChain::CreateDepthBuffer(VkCommandPool& commandPool)
	{
		spdlog::info("Create depth buffer");
		this->commandPool = commandPool;

		VkFormat depthFormat = VkFormatExtension::FindDepthBufferFormat(physicalDevice);
		depthBufferModel = CreateRef<ImageModel>();

		depthBufferModel->image = AImage(physicalDevice, logicalDevice, graphicsQueue, commandPool).Create(
			swapChainData->extent.width, swapChainData->extent.height, depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			depthBufferModel->imageMemory);

		AImageView().Create(logicalDevice, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			depthBufferModel->image, depthBufferModel->imageView, rollback);

		rollback->Add([this]()
		{
			AImage(physicalDevice, logicalDevice, graphicsQueue, this->commandPool).Destroy(depthBufferModel->image);
			VkMemoryExtension::FreeDeviceMemory(logicalDevice, depthBufferModel->imageMemory);
			depthBufferModel.reset();
		});
	}

	void SwapChain::CreateFrameBuffers(VkRenderPass& renderPass)
	{
		this->renderPass = renderPass;
		AFrameBuffer().Create(logicalDevice, renderPass, *swapChainData.get(), depthBufferModel);
		rollback->Add([this]() { AFrameBuffer().Dispose(logicalDevice, swapChainData->frameBuffers); });
	}

	void SwapChain::Dispose()
	{
		rollback->Dispose();
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

	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
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
		VkPresentModeKHR highQualityMode = VK_PRESENT_MODE_MAILBOX_KHR;
		VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& mode : availableModes)
		{
			if (mode == highQualityMode)
			{
				return mode;
			}
		}

		spdlog::warn("Present mode is not available: {0} Default mode will be used {1}", highQualityMode, defaultMode);
		return defaultMode;
	}

}