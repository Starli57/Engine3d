#include "Pch.h"
#include "AImage.h"
#include "SharedLib/CustomAssert.h"
#include "Rendering/Vulkan/Utilities/VkMemoryUtility.h"
#include "Rendering/Vulkan/Buffers/ABuffer.h"

namespace AVulkan
{
    AImage::AImage(Ref<VulkanModel> vulkanModel, VkCommandPool& commandPool)
        : vulkanModel(vulkanModel), commandPool(commandPool)
    {
    }

    VkImage AImage::Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.usage = usage;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkImage image = nullptr;
        auto createStatus = vkCreateImage(vulkanModel->logicalDevice, &imageInfo, nullptr, &image);
        CAssert::Check(createStatus == VK_SUCCESS, "Failed to create vk image");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(vulkanModel->logicalDevice, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkMemoryUtility::FindMemoryType(vulkanModel->physicalDevice, memRequirements.memoryTypeBits, properties);

        auto allocateStatus = vkAllocateMemory(vulkanModel->logicalDevice, &allocInfo, nullptr, &imageMemory);
        CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate vk image memory");

        vkBindImageMemory(vulkanModel->logicalDevice, image, imageMemory, 0);
        return image;
    }

    void AImage::CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height) const
    {
        ABuffer bufferUtility;
        auto commandBuffer = bufferUtility.BeginCommandBuffer(vulkanModel->logicalDevice, commandPool);

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
        bufferUtility.SubmitCommandBuffer(vulkanModel->graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(vulkanModel->logicalDevice, commandPool, 1, &commandBuffer);
    }

    void AImage::TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const
    {
        ABuffer bufferUtility;
        auto commandBuffer = bufferUtility.BeginCommandBuffer(vulkanModel->logicalDevice, commandPool);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
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
        else 
        {
            throw std::invalid_argument("Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        vkEndCommandBuffer(commandBuffer);
        bufferUtility.SubmitCommandBuffer(vulkanModel->graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(vulkanModel->logicalDevice, commandPool, 1, &commandBuffer);
    }

    void AImage::Destroy(VkImage& image) const
    {
        vkDestroyImage(vulkanModel->logicalDevice, image, nullptr);
    }
}