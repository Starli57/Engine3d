#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"

namespace VulkanApi
{
    void CreateImage(VulkanContext* vulkanContext,
                        uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                        VkSampleCountFlagBits msaa, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void DestroyImage(const VkDevice& logicalDevice, const VkImage& image);
    
    void CopyBufferToImage(VulkanContext* vulkanContext,
                           const VkBuffer& buffer, const VkImage& image, uint32_t width, uint32_t height, VkCommandPool& commandPool);
    
    void TransitionImageLayout(const VkCommandBuffer& commandBuffer, const uint32_t mipLevels,
                               const VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask);

    void CreateImageView(const VkDevice& logicalDevice, const uint32_t mipLevels, const VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
                         const VkImage& image, VkImageView& imageView);
    
    void DestroyImageView(const VkDevice& logicalDevice, const VkImageView& imageView);

    void DisposeImageModel(const VkDevice& logicalDevice, const ImageModel* imageModel);
}