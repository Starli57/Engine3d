#include "Pch.h"
#include "AImage.h"
#include "Architecture/CustomAssert.h"
#include "Rendering/Vulkan/Utilities/VkMemoryUtility.h"

namespace AVulkan
{
    VkImage AImage::Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, uint32_t width, uint32_t height,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
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
        auto createStatus = vkCreateImage(logicalDevice, &imageInfo, nullptr, &image);
        CAssert::Check(createStatus == VK_SUCCESS, "Failed to create vk image");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(logicalDevice, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkMemoryUtility::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        auto allocateStatus = vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &imageMemory);
        CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate vk image memory");

        vkBindImageMemory(logicalDevice, image, imageMemory, 0);
        return image;
    }

    void AImage::Destroy(VkDevice& logicalDevice, VkImage& image) const
    {
        vkDestroyImage(logicalDevice, image, nullptr);
    }
}