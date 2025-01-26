#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"

namespace VkUtils
{
    VkImage CreateImage(VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice,
        uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkSampleCountFlagBits msaa, VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory);
    void DestroyImage(const VkDevice& logicalDevice, const VkImage& image);
    
    void CopyBufferToImage(VkDevice& logicalDevice, VkQueue& graphicsQueue,
                           const VkBuffer& buffer, const VkImage& image, uint32_t width, uint32_t height, VkCommandPool& commandPool);
    void TransitionImageLayout(VkDevice& logicalDevice, const VkCommandBuffer& commandBuffer, VkQueue& graphicsQueue,
                               const VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask);

    void CreateImageView(const VkDevice& logicalDevice, const VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
                         const VkImage& image, VkImageView& imageView);
    void DestroyImageView(const VkDevice& logicalDevice, const VkImageView& imageView);

    void DisposeImageModel(VkDevice& logicalDevice, Ref<AVulkan::ImageModel> imageModel);
}