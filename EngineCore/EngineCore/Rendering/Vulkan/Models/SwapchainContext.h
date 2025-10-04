#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Core/Ref.h"

namespace VulkanApi
{
	struct SwapchainContext
	{
	public:

		VkSwapchainKHR swapChain;
		VkSwapchainKHR oldSwapChain;//todo: add usage to reuse resources

		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR presentMode;
		
		VkExtent2D extent;

		uint32_t imagesCount;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		ImageModel* depthBufferModel;//todo: replace to render passes
		ImageModel* msaaColorSample;//todo: replace to render passes
		ImageModel* msaaDepthSample;//todo: replace to render passes
	};
}