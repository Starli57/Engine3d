#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace VulkanApi
{
    struct SwapchainSurfaceSettings
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
}