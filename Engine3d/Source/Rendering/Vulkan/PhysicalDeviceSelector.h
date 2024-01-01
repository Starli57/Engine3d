#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"
#include "SwapChainInterface.h"

class PhysicalDeviceSelector
{
public:
	PhysicalDeviceSelector();
	~PhysicalDeviceSelector();

	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface);
    std::vector<VkPhysicalDevice> GetDevicesList(VkInstance instance);
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	SwapChainInterface* swapChainInterface;

	uint64_t CalculateRenderingScore(VkPhysicalDevice device);

	bool DoSupportQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool DoSupportPhysicalDeviceExtensions(VkPhysicalDevice device);
	bool DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);
};

