#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "SwapChainDetails.h"

class SwapChainInterface
{
public:
	
	void CreateSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);

	SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
	void GetSwapChainColorFormats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);
	void GetSwapChainPresentModes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& presentModes);

	VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	bool DoSupportSwapChain(SwapChainDetails details);
	bool DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);

};

