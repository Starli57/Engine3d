#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
    void CreateTextureSampler(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkSampler& textureSampler);
}