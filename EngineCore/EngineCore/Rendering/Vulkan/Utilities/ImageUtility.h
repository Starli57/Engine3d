#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"

namespace VkUtils
{
    VkImage CreateImage(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
        uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkSampleCountFlagBits msaa, VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory);
    void DestroyImage(VkDevice& logicalDevice, VkImage& image);
    
    void CopyBufferToImage(VkDevice& logicalDevice, VkQueue& graphicsQueue,
        VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height, VkCommandPool& commandPool);
    void TransitionImageLayout(VkDevice& logicalDevice, VkCommandBuffer& commandBuffer, VkQueue& graphicsQueue,
        VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask);

    void CreateImageView(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
        VkImage& image, VkImageView& imageView);
    void DestroyImageView(VkDevice& logicalDevice, VkImageView& imageView);

    void DisposeImageModel(VkDevice& logicalDevice, Ref<AVulkan::ImageModel> imageModel);
}