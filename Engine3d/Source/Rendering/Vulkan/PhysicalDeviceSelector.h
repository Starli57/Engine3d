#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "QueueFamilyIndices.h"

class PhysicalDeviceSelector
{
public:
	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance);
    std::vector<VkPhysicalDevice> GetListOfPhysicalDevices(VkInstance instance);
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device);

private:

    int CalculateRenderingScore(VkPhysicalDevice device);
};

