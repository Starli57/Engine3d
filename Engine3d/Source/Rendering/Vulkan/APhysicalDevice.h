#pragma once

#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"
#include "ASwapChain.h"

class APhysicalDevice
{
public:
	std::vector<VkPhysicalDevice> GetDevicesList(VkInstance& instance) const;
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(VkInstance& instance, VkSurfaceKHR& surface) const;

	VkPhysicalDevice GetBestRenderingDevice(VkInstance& instance, VkSurfaceKHR& surface) const;
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) const;

	void PrintDebugInformation(VkPhysicalDevice& device, VkSurfaceKHR& surface) const;

private:
	uint64_t CalculateRenderingScore(VkPhysicalDevice& device) const;

	bool DoSupportQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) const;
	bool DoSupportPhysicalDeviceExtensions(VkPhysicalDevice& device) const;
	bool DoSupportSwapChain(VkPhysicalDevice& device, VkSurfaceKHR& surface) const;
};

