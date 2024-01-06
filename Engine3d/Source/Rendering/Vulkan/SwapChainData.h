#pragma once

#include <GLFW/glfw3.h>

#include <vector>

struct SwapChainData 
{
public:
	VkSwapchainKHR swapChain;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

};