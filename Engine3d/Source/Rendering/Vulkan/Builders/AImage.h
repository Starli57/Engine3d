#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
    class AImage
    {
    public:
        VkImage Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const;

        void Destroy(VkDevice& logicalDevice, VkImage& image) const;
    };
}
