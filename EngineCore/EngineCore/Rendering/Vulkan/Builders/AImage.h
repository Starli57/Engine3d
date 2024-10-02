#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
    class AImage
    {
    public:
        AImage(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue);

        VkImage Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
            VkSampleCountFlagBits msaa, VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const;

        void CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height, VkCommandPool& commandPool) const;

        void TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool& commandPool) const;

        void Destroy(VkImage& image) const;

    private:
        VkPhysicalDevice& physicalDevice;
        VkDevice& logicalDevice;
        VkQueue& graphicsQueue;
    };
}