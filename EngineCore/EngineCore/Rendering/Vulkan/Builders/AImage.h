#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Ref.h"

namespace AVulkan
{
    class AImage
    {
    public:
        AImage(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool);

        VkImage Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const;

        void CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height) const;

        void TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;

        void Destroy(VkImage& image) const;

    private:
        VkPhysicalDevice& physicalDevice;
        VkDevice& logicalDevice;
        VkQueue& graphicsQueue;
        VkCommandPool& commandPool;
    };
}