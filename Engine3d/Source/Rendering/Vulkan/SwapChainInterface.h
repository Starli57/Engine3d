#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "SwapChainDetails.h"

class SwapChainInterface
{
public:
	SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);
};

