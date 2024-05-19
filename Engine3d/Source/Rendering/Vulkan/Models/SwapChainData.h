#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace AVulkan
{
	struct SwapChainData
	{
	public:

		VkSwapchainKHR swapChain;
		VkSwapchainKHR oldSwapChain;//todo: add usage to reuse resources

		VkSurfaceFormatKHR surfaceFormat;
		VkFormat imageFormat;
		VkExtent2D extent;

		uint32_t imagesCount;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		std::vector<VkFramebuffer> frameBuffers;
	};
}