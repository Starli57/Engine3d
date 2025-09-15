#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace VulkanApi
{
    struct SwapChainSurfaceSettings
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
}