#pragma once

#include <GLFW/glfw3.h>

#include "SwapChainData.h"

class AImageView
{
public:
	void Create(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
	void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;
};

