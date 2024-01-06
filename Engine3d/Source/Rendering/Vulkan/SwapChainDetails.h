#pragma once

#include <GLFW/glfw3.h>

#include <vector>

struct SwapChainDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};