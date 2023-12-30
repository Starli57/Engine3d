#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value();
    }
};

class PhysicalDeviceSelector
{
public:
	VkPhysicalDevice GetBestRenderingDevice(VkInstance instance);
    std::vector<VkPhysicalDevice> GetListOfPhysicalDevices(VkInstance instance);

private:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    int CalculateRenderingScore(VkPhysicalDevice device);
};

