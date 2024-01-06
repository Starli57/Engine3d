#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class ImageViewInterface
{
public:
	std::vector<VkImageView> swapChainImageViews;

	void CreateImageViews(VkDevice logicalDevice, std::vector<VkImage>& swapChainImages, VkFormat imageFormat);
	void Dispose(VkDevice logicalDevice);
};

