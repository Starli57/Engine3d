#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "SwapChainData.h"
#include "SwapChainDetails.h"
#include "QueueFamilyIndices.h"

class SwapChainInterface
{
public:
	SwapChainData CreateSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
		VkSurfaceKHR surface, QueueFamilyIndices& physicalDeviceQueueIndices);
	void DestroySwapChain(VkDevice logicalDevice, SwapChainData& swapChainData);

	SwapChainDetails GetSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void GetSwapChainColorFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);
	void GetSwapChainPresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& presentModes);

	bool DoSupportSwapChain(SwapChainDetails details);
	bool DoSupportSwapChain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

private:
	VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkExtent2D ChooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);

};

