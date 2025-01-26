#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
    void CreateTextureSampler(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, VkSampler& textureSampler);
}