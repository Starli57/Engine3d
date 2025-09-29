#include "EngineCore/Pch.h"
#include "SwapchainManager.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkPhysicalDeviceWrapper.h"
#include "ApiWrappers/VkImageWrapper.h"
#include "ApiWrappers/SwapchainUtility.h"

namespace VulkanApi
{
	SwapchainManager::SwapchainManager(VulkanContext* vulkanContext, SwapchainContext* swapchainContext) :
		vulkanContext(vulkanContext), swapchainContext(swapchainContext)
	{
		physicalDeviceQueueIndices = GetQueueFamilies(vulkanContext->physicalDevice, vulkanContext->windowSurface);
	}

	SwapchainManager::~SwapchainManager()
	{
	}

	void SwapchainManager::Recreate()
	{
		Dispose();

		CreateSwapchain();
		CreateSwapChainImageViews();
		CreateDepthBuffer();
		CreateMSAAColorResources();
		CreateMSAADepthResources();
	}

	void SwapchainManager::CreateSwapchain()
	{
		spdlog::info("Create swap chain");
		
		auto details = GetSwapChainDetails(vulkanContext->physicalDevice, vulkanContext->windowSurface);
		Engine::CAssert::Check(DoSupportSwapChain(details), "Swap chains are not supported");

		swapchainContext->surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(*vulkanContext->window, details.capabilities);

		swapchainContext->imagesCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && swapchainContext->imagesCount > details.capabilities.maxImageCount)
		{
			swapchainContext->imagesCount = details.capabilities.maxImageCount;
		}

		spdlog::info("Swap chain images count: {0}", swapchainContext->imagesCount);


		VkSwapchainCreateInfoKHR createInfo{};
		SetupSwapChainInfo(createInfo, vulkanContext->windowSurface, extent, presentMode, swapchainContext->surfaceFormat, 
			details.capabilities, physicalDeviceQueueIndices, swapchainContext->imagesCount);

		auto createStatus = vkCreateSwapchainKHR(vulkanContext->logicalDevice, &createInfo, nullptr, &swapchainContext->swapChain);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create swap chain, status: " + createStatus);

		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, &swapchainContext->imagesCount, nullptr);
		swapchainContext->images.resize(swapchainContext->imagesCount);
		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, &swapchainContext->imagesCount, swapchainContext->images.data());

		vulkanContext->imageFormat = swapchainContext->surfaceFormat.format;
		swapchainContext->extent = extent;
	}

	void SwapchainManager::CreateSwapChainImageViews() const
	{
		swapchainContext->imageViews.resize(swapchainContext->images.size());
		for (int i = 0; i < swapchainContext->imageViews.size(); i++)
		{
			CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
				swapchainContext->images[i], swapchainContext->imageViews[i]);
		}
	}

	void SwapchainManager::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");

		swapchainContext->depthBufferModel = new ImageModel();

		CreateImage(
			vulkanContext, swapchainContext->extent.width, swapchainContext->extent.height, 1, vulkanContext->depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_SAMPLE_COUNT_1_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapchainContext->depthBufferModel->image,
			swapchainContext->depthBufferModel->imageMemory);

		CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			swapchainContext->depthBufferModel->image, swapchainContext->depthBufferModel->imageView);
	}

	void SwapchainManager::CreateMSAAColorResources() const
	{
		spdlog::info("Create msaa color buffer");
		swapchainContext->msaaColorSample = new ImageModel();
		CreateImage(vulkanContext, 
			swapchainContext->extent.width, swapchainContext->extent.height, 1, vulkanContext->imageFormat,
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			vulkanContext->msaa, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapchainContext->msaaColorSample->image,
			swapchainContext->msaaColorSample->imageMemory);

		CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
			swapchainContext->msaaColorSample->image, swapchainContext->msaaColorSample->imageView);
	}

	void SwapchainManager::CreateMSAADepthResources() const
	{
		spdlog::info("Create msaa depth buffer");
		
		swapchainContext->msaaDepthSample = new ImageModel();
		CreateImage(vulkanContext, 
			swapchainContext->extent.width, swapchainContext->extent.height, 1, vulkanContext->depthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			vulkanContext->msaa, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapchainContext->msaaDepthSample->image,
			swapchainContext->msaaDepthSample->imageMemory);

		CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			swapchainContext->msaaDepthSample->image, swapchainContext->msaaDepthSample->imageView);
	}

	void SwapchainManager::Dispose() const
	{
		spdlog::info("Dispose swapchain");

		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->msaaDepthSample);
		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->msaaColorSample);
		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->depthBufferModel);

		for (int i = 0; i < swapchainContext->imageViews.size(); i++)
			DestroyImageView(vulkanContext->logicalDevice, swapchainContext->imageViews.at(i));
		swapchainContext->imageViews.clear();

		vkDestroySwapchainKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, nullptr);
	}
	
	void SwapchainManager::SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
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

	VkSurfaceFormatKHR SwapchainManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
	{
		//todo: chose format by player settings or most often used

		for (const auto& data : availableFormats)
		{
			if (data.format == VK_FORMAT_R8G8B8A8_UNORM && data.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				spdlog::info("Chosen color format: {0}", static_cast<int>(data.format));
				return data;
			}
		}

		spdlog::warn("Fallback color format: {0}", static_cast<int>(availableFormats[0].format));
		return availableFormats[0];
	}

	VkExtent2D SwapchainManager::ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const
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

	VkPresentModeKHR SwapchainManager::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const
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
