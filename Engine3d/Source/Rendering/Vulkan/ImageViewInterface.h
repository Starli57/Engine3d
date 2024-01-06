#pragma once

#include <GLFW/glfw3.h>

#include "SwapChainData.h"

class ImageViewInterface
{
public:
	void CreateImageViews(VkDevice logicalDevice, SwapChainData& swapChainData);
	void Dispose(VkDevice logicalDevice, SwapChainData& swapChainData);
};

