#include "EngineCore/Pch.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"

#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"

namespace VkUtils
{
    void CreateImage(const Ref<AVulkan::VulkanContext>& vulkanContext,
                        const uint32_t width, const uint32_t height, const uint32_t mipLevels, const VkFormat format,
                        const VkImageTiling tiling, const VkImageUsageFlags usage,
                        const VkSampleCountFlagBits msaa, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.usage = usage;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.samples = msaa;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const auto createStatus = vkCreateImage(vulkanContext->logicalDevice, &imageInfo, nullptr, &image);
        CAssert::Check(createStatus == VK_SUCCESS, "Failed to create vk image");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(vulkanContext->logicalDevice, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(vulkanContext->physicalDevice, memRequirements.memoryTypeBits, properties);

        const auto allocateStatus = vkAllocateMemory(vulkanContext->logicalDevice, &allocInfo, nullptr, &imageMemory);
        CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate vk image memory");

        vkBindImageMemory(vulkanContext->logicalDevice, image, imageMemory, 0);
    }

    void CopyBufferToImage(const Ref<AVulkan::VulkanContext>& vulkanContext, const VkBuffer& buffer, const VkImage& image, const uint32_t width, const uint32_t height, VkCommandPool& commandPool)
    {
        auto commandBuffer = BeginCommandBuffer(vulkanContext->logicalDevice, commandPool);

        VkBufferImageCopy imageRegion{};
        imageRegion.bufferOffset = 0;
        imageRegion.bufferRowLength = 0;
        imageRegion.bufferImageHeight = 0;

        imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageRegion.imageSubresource.mipLevel = 0;
        imageRegion.imageSubresource.baseArrayLayer = 0;
        imageRegion.imageSubresource.layerCount = 1;

        imageRegion.imageOffset = { 0, 0, 0 };
        imageRegion.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);

        vkEndCommandBuffer(commandBuffer);
        SubmitCommandBuffer(vulkanContext->graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(vulkanContext->logicalDevice, commandPool, 1, &commandBuffer);
    }

    void TransitionImageLayout(const VkCommandBuffer& commandBuffer, const uint32_t mipLevels, const VkImage& image,
        const VkImageLayout oldLayout, const VkImageLayout newLayout, const VkImageAspectFlags aspectMask)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = aspectMask;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = 0; // No access required for presentation

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }
        else 
        {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    void DestroyImage(const VkDevice& logicalDevice, const VkImage& image)
    {
        vkDestroyImage(logicalDevice, image, nullptr);
    }

    void CreateImageView(const VkDevice& logicalDevice, const uint32_t mipLevels, const VkFormat& imageFormat, const VkImageAspectFlags imageAspectFlags,
        const VkImage& image, VkImageView& imageView)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = imageFormat;
        
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = imageAspectFlags;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = mipLevels;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        const auto createStatus = vkCreateImageView(logicalDevice, &createInfo, nullptr, &imageView);
        CAssert::Check(createStatus == VK_SUCCESS, "Image view can't be created, status: " + createStatus);
    }

    void DestroyImageView(const VkDevice& logicalDevice, const VkImageView& imageView)
    {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }

    void DisposeImageModel(VkDevice& logicalDevice, Ref<AVulkan::ImageModel> imageModel)
    {
        VkUtils::DestroyImage(logicalDevice, imageModel->image);
        VkUtils::DestroyImageView(logicalDevice, imageModel->imageView);
        VkUtils::FreeDeviceMemory(logicalDevice, imageModel->imageMemory);
        imageModel.reset();
    }
}