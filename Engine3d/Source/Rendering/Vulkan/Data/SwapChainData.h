#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace AVulkan
{
	struct SwapChainData
	{
	public:
		VkSwapchainKHR swapChain;
		VkFormat imageFormat;
		VkExtent2D extent;

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		std::vector<VkFramebuffer> framebuffers;
		std::vector<VkCommandBuffer> commandbuffers;

	};
}