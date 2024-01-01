#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"

class PhysicalDeviceSelector
{
public:
	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface);
    std::vector<VkPhysicalDevice> GetDevicesList(VkInstance instance);
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

private:

    int CalculateRenderingScore(VkPhysicalDevice device);
};

