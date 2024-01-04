#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"
#include "SwapChainInterface.h"

class PhysicalDeviceInterface
{
public:
	PhysicalDeviceInterface();
	~PhysicalDeviceInterface();

	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface);
    std::vector<VkPhysicalDevice> GetDevicesList(VkInstance instance);
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(VkInstance instance, VkSurfaceKHR surface);
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	SwapChainInterface* swapChainInterface;

	uint64_t CalculateRenderingScore(VkPhysicalDevice device);

	bool DoSupportQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool DoSupportPhysicalDeviceExtensions(VkPhysicalDevice device);
	bool DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface);
};

