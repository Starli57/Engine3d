#pragma once

#include <GLFW/glfw3.h>

#include <vector>

struct SwapChainSurfaceSettings 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};