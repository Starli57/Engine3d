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

        void CopyBufferToImage(VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool,
            VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height) const;

        void Destroy(VkDevice& logicalDevice, VkImage& image) const;
    };
}