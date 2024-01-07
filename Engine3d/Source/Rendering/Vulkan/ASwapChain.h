#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "SwapChainData.h"
#include "SwapChainSurfaceSettings.h"
#include "QueueFamilyIndices.h"

class ASwapChain
{
public:
	SwapChainData Create(GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
		VkSurfaceKHR& surface, QueueFamilyIndices& physicalDeviceQueueIndices) const;
	void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;

	SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;
	void GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats) const;
	void GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes) const;

	bool DoSupportSwapChain(SwapChainSurfaceSettings& surfaceSettings) const;
	bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;

private:
	VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

};

