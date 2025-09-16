#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
    void CreateTextureSampler(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, VkSampler& textureSampler);
}