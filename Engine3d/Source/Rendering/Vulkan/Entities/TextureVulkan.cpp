#include "Pch.h"
#include "TextureVulkan.h"

#include "Architecture/CustomAssert.h"
#include "Rendering/Vulkan/Buffers/ABuffer.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/Utilities/VkImageViewUtility.h"

namespace AVulkan
{
    TextureVulkan::TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
        VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& filePath) 
        : Texture(filePath), physicalDevice(physicalDevice), logicalDevice(logicalDevice), 
                             graphicsQueue(graphicsQueue), commandPool(commandPool)
    {
        CreateImage(filePath);
        CreateImageView();
    }

    TextureVulkan::~TextureVulkan()
    {
        VkImageViewUtility::Destroy(logicalDevice, *imageView.get());
        vkDestroyImage(logicalDevice, *image.get(), nullptr);
        vkFreeMemory(logicalDevice, *imageMemory.get(), nullptr);
    }

    //todo: make async
    void TextureVulkan::CreateImage(const std::string& filePath)
    {
        int width;
        int height;
        int texChannels;
        uint64_t bitesPerPixel = 4;
        VkDeviceSize imageSize;

        auto pixels = stbi_load(filePath.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        imageSize = width * height * bitesPerPixel;

        CAssert::Check(pixels != nullptr, "Failed to load texture image: " + filePath);

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        VkBufferUsageFlags usageFlagsStaging = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryFlagsStaging = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        ABuffer().Create(physicalDevice, logicalDevice, imageSize,
            usageFlagsStaging, memoryFlagsStaging, stagingBuffer, stagingMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(logicalDevice, stagingMemory);

        stbi_image_free(pixels);

        //todo: find suitable format firstly
        VkFormat imgFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        
        AImage imageUtility(physicalDevice, logicalDevice, graphicsQueue, commandPool);
        
        image = CreateRef<VkImage>(imageUtility.Create(width, height, imgFormat, 
            tiling, usageFlags, memoryFlags, *imageMemory.get()));

        imageUtility.TransitionImageLayout(image, imgFormat, 
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        imageUtility.CopyBufferToImage(stagingBuffer, *image.get(), width, height);

        imageUtility.TransitionImageLayout(image, imgFormat,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);
    }

    void TextureVulkan::CreateImageView()
    {
        //todo: find suitable format firstly
        VkFormat imgFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageViewUtility::Create(logicalDevice, imgFormat, aspectFlags, *image.get(), *imageView.get());

    }
}