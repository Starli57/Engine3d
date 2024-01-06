#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"
#include "SwapChainInterface.h"

class PhysicalDeviceInterface
{
public:
	std::vector<VkPhysicalDevice> GetDevicesList(VkInstance instance);
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(VkInstance instance, VkSurfaceKHR surface);

	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface);
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	uint64_t CalculateRenderingScore(VkPhysicalDevice device);

	bool DoSupportQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool DoSupportPhysicalDeviceExtensions(VkPhysicalDevice device);
	bool DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);
};

