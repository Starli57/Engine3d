#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace AVulkan
{
	struct SwapChainData
	{
	public:
		VkSwapchainKHR swapChain;

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		VkFormat imageFormat;
		VkExtent2D extent;

	};
}