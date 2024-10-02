#include "EngineCore/Pch.h"
#include "TextureVulkan.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace AVulkan
{
    TextureVulkan::TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue,
        VkCommandPool& commandPool, Ref<VulkanConfiguration> rendererConfig, const std::filesystem::path& textureFilePath, Ref<Rollback> rollback)
        : Texture(textureFilePath), physicalDevice(physicalDevice), logicalDevice(logicalDevice),
          graphicsQueue(graphicsQueue), commandPool(commandPool), rendererConfig(rendererConfig), rollback(rollback)
    {
        imageModel = CreateRef<ImageModel>();

        CreateImage(textureFilePath, rollback);
        CreateImageView();
    }

    TextureVulkan::~TextureVulkan()
    {
    }

    //todo: make async
    void TextureVulkan::CreateImage(const std::filesystem::path& textureFilePath, Ref<Rollback> rollback)
    {
        int width;
        int height;
        int texChannels;
        uint64_t bitesPerPixel = 4;
        VkDeviceSize imageSize;

        auto pixels = stbi_load(textureFilePath.string().c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        imageSize = width * height * bitesPerPixel;

        if (pixels == nullptr)
        {
            std::filesystem::path fullPath = std::filesystem::absolute(textureFilePath);
            CAssert::Check(pixels != nullptr, "Failed to load texture image: " + fullPath.string());
        }
        else
        {
            spdlog::debug("Image loaded: " + textureFilePath.string());
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        VkBufferUsageFlags usageFlagsStaging = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryFlagsStaging = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkUtils::CreateBuffer(physicalDevice, logicalDevice, imageSize,
            usageFlagsStaging, memoryFlagsStaging, stagingBuffer, stagingMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(logicalDevice, stagingMemory);

        stbi_image_free(pixels);

        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        
        AImage imageUtility(physicalDevice, logicalDevice, graphicsQueue);
        
        imageModel->image = imageUtility.Create(width, height, rendererConfig->imageFormat,
            tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, imageModel->imageMemory);

        imageUtility.TransitionImageLayout(imageModel->image, rendererConfig->imageFormat,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandPool);

        imageUtility.CopyBufferToImage(stagingBuffer, imageModel->image, width, height, commandPool);

        imageUtility.TransitionImageLayout(imageModel->image, rendererConfig->imageFormat,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandPool);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);

        rollback->Add([this]() { vkDestroyImage(logicalDevice, imageModel->image, nullptr); });
        rollback->Add([this]() { vkFreeMemory(logicalDevice, imageModel->imageMemory, nullptr); });
    }

    void TextureVulkan::CreateImageView()
    {
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        AImageView().Create(logicalDevice, rendererConfig->imageFormat, aspectFlags, imageModel->image, imageModel->imageView, rollback);

    }
}