#include "EngineCore/Pch.h"
#include "VkSwapchainWrapper.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkPhysicalDeviceWrapper.h"
#include "ApiWrappers/VkImageWrapper.h"

namespace VulkanApi
{
	void RecreateSwapchain(VulkanContext* vulkanContext)
	{
		DisposeSwapchain(vulkanContext);

		CreateSwapchain(vulkanContext);
		CreateSwapchainImageViews(vulkanContext);
		CreateDepthBuffer(vulkanContext);
		CreateMSAAColorResources(vulkanContext);
		CreateMSAADepthResources(vulkanContext);
	}

	void CreateSwapchain(VulkanContext* vulkanContext)
	{
		spdlog::info("Create swap chain");
		auto& swapchainContext = vulkanContext->swapchainContext;

		SwapchainSurfaceSettings swapchainSurfaceSettings;
		SetSwapchainDetails(vulkanContext, swapchainSurfaceSettings);
		Engine::CAssert::Check(DoSupportSwapchain(vulkanContext), "Swap chains are not supported");

		SetSurfaceFormat(swapchainSurfaceSettings.formats, swapchainContext);
		SetPresentMode(swapchainSurfaceSettings.presentModes, swapchainContext);
		SetSwapExtent(swapchainSurfaceSettings.capabilities, vulkanContext);

		swapchainContext->imagesCount = swapchainSurfaceSettings.capabilities.minImageCount + 1;
		if (swapchainSurfaceSettings.capabilities.maxImageCount > 0 && swapchainContext->imagesCount > swapchainSurfaceSettings.capabilities.maxImageCount)
		{
			swapchainContext->imagesCount = swapchainSurfaceSettings.capabilities.maxImageCount;
		}

		spdlog::info("Swap chain images count: {0}", vulkanContext->swapchainContext->imagesCount);
		
		VkSwapchainCreateInfoKHR createInfo{};
		SetSwapchainInfo(vulkanContext, swapchainSurfaceSettings.capabilities, createInfo);

		auto createStatus = vkCreateSwapchainKHR(vulkanContext->logicalDevice, &createInfo, nullptr, &swapchainContext->swapChain);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create swap chain, status: " + createStatus);

		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, &swapchainContext->imagesCount, nullptr);
		swapchainContext->images.resize(swapchainContext->imagesCount);
		vkGetSwapchainImagesKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, &swapchainContext->imagesCount, swapchainContext->images.data());

		vulkanContext->imageFormat = swapchainContext->surfaceFormat.format;
	}

	void CreateSwapchainImageViews(VulkanContext* vulkanContext)
	{
		auto& swapchainContext = vulkanContext->swapchainContext;
		swapchainContext->imageViews.resize(swapchainContext->images.size());
		for (int i = 0; i < swapchainContext->imageViews.size(); i++)
		{
			CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->imageFormat, VK_IMAGE_ASPECT_COLOR_BIT,
				swapchainContext->images[i], swapchainContext->imageViews[i]);
		}
	}

	void CreateDepthBuffer(VulkanContext* vulkanContext)
	{
		spdlog::info("Create depth buffer");
		auto& swapchainContext = vulkanContext->swapchainContext;
		swapchainContext->depthBufferModel = new ImageModel();

		CreateImage(
			vulkanContext, swapchainContext->extent.width, swapchainContext->extent.height, 1, vulkanContext->depthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_SAMPLE_COUNT_1_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			swapchainContext->depthBufferModel->image,
			swapchainContext->depthBufferModel->imageMemory);

		CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			swapchainContext->depthBufferModel->image, swapchainContext->depthBufferModel->imageView);
	}

	void CreateMSAAColorResources(VulkanContext* vulkanContext)
	{
		spdlog::info("Create msaa color buffer");
		auto& swapchainContext = vulkanContext->swapchainContext;
		
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

	void CreateMSAADepthResources(VulkanContext* vulkanContext)
	{
		spdlog::info("Create msaa depth buffer");
		auto& swapchainContext = vulkanContext->swapchainContext;
		
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

	void DisposeSwapchain(const VulkanContext* vulkanContext)
	{
		spdlog::info("Dispose swapchain");

		auto& swapchainContext = vulkanContext->swapchainContext;
		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->msaaDepthSample);
		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->msaaColorSample);
		DisposeImageModel(vulkanContext->logicalDevice, swapchainContext->depthBufferModel);

		for (int i = 0; i < swapchainContext->imageViews.size(); i++)
			DestroyImageView(vulkanContext->logicalDevice, swapchainContext->imageViews.at(i));
		swapchainContext->imageViews.clear();

		vkDestroySwapchainKHR(vulkanContext->logicalDevice, swapchainContext->swapChain, nullptr);
	}
	
	void SetSwapchainInfo(VulkanContext* vulkanContext, VkSurfaceCapabilitiesKHR& capabilities, VkSwapchainCreateInfoKHR& outCreateInfo)
	{
		auto& swapchainContext = vulkanContext->swapchainContext;
		
		outCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		outCreateInfo.surface = vulkanContext->windowSurface;
		outCreateInfo.imageExtent = swapchainContext->extent;
		outCreateInfo.presentMode = swapchainContext->presentMode;
		outCreateInfo.minImageCount = swapchainContext->imagesCount;

		outCreateInfo.imageFormat = swapchainContext->surfaceFormat.format;
		outCreateInfo.imageColorSpace = swapchainContext->surfaceFormat.colorSpace;

		outCreateInfo.imageArrayLayers = 1;
		outCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		outCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		outCreateInfo.preTransform = capabilities.currentTransform;
		outCreateInfo.clipped = VK_TRUE;
		outCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vulkanContext->queueFamilies.graphicsFamily != vulkanContext->queueFamilies.presentationFamily)
		{
			uint32_t queueFamilyIndices[] = { vulkanContext->queueFamilies.graphicsFamily.value(), vulkanContext->queueFamilies.presentationFamily.value() };

			outCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			outCreateInfo.queueFamilyIndexCount = 2;
			outCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			outCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			outCreateInfo.queueFamilyIndexCount = 0;
			outCreateInfo.pQueueFamilyIndices = nullptr;
		}
	}

	void SetSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, SwapchainContext* swapchainContext)
	{
		//todo: chose format by player settings or most often used

		for (const auto& format : availableFormats)
		{
			if (format.format == VK_FORMAT_R8G8B8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				spdlog::info("Chosen color format: {0}", static_cast<int>(format.format));
				swapchainContext->surfaceFormat = format;
				return;
			}
		}

		spdlog::warn("Fallback color format: {0}", static_cast<int>(availableFormats[0].format));
		swapchainContext->surfaceFormat = availableFormats[0];
	}

	void SetSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VulkanContext* vulkanContext)
	{
		auto& extent = vulkanContext->swapchainContext->extent;
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			extent = capabilities.currentExtent;
			return;
		}
		
		int width, height;
		glfwGetFramebufferSize(vulkanContext->window, &width, &height);

		extent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	}

	void SetPresentMode(const std::vector<VkPresentModeKHR>& availableModes, SwapchainContext* swapchainContext) 
	{
		constexpr VkPresentModeKHR highQualityMode = VK_PRESENT_MODE_MAILBOX_KHR;
		constexpr VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& mode : availableModes)
		{
			if (mode == highQualityMode)
			{
				swapchainContext->presentMode = mode;
				return;
			}
		}

		spdlog::warn("Present mode is not available: {0} Default mode will be used {1}", static_cast<int>(highQualityMode), static_cast<int>(defaultMode));
		swapchainContext->presentMode = defaultMode;
	}

	void SetSwapchainDetails(const VulkanContext* vulkanContext, SwapchainSurfaceSettings& surfaceSettings)
	{
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanContext->physicalDevice, vulkanContext->windowSurface, &surfaceSettings.capabilities);

		SetSwapchainColorFormats(vulkanContext, surfaceSettings.formats);
		SetSwapchainPresentModes(vulkanContext, surfaceSettings.presentModes);
	}

	void SetSwapchainColorFormats(const VulkanContext* vulkanContext, std::vector<VkSurfaceFormatKHR>& outFormats)
	{
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanContext->physicalDevice, vulkanContext->windowSurface, &formatCount, nullptr);

		outFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanContext->physicalDevice, vulkanContext->windowSurface, &formatCount, outFormats.data());
	}

	void SetSwapchainPresentModes(const VulkanContext* vulkanContext, std::vector<VkPresentModeKHR>& outPresentModes)
	{
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanContext->physicalDevice, vulkanContext->windowSurface, &presentModeCount, nullptr);

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanContext->physicalDevice, vulkanContext->windowSurface, &presentModeCount, presentModes.data());

		outPresentModes.resize(presentModeCount);
		for(uint32_t i = 0; i < presentModeCount; i++) outPresentModes[i] = presentModes[i];
	}
	
	bool DoSupportSwapchain(const SwapchainSurfaceSettings& details)
	{
		return !details.formats.empty() && !details.presentModes.empty();
	}

	bool DoSupportSwapchain(const VulkanContext* vulkanContext)
	{
		SwapchainSurfaceSettings details;
		SetSwapchainDetails(vulkanContext, details);
		return DoSupportSwapchain(details);
	}
}
