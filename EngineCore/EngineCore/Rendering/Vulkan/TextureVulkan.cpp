#include "EngineCore/Pch.h"
#include "TextureVulkan.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace AVulkan
{
    TextureVulkan::TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue,
        VkCommandPool& commandPool, Ref<VulkanConfiguration> rendererConfig, const std::filesystem::path& textureFilePath)
        : Texture(textureFilePath), physicalDevice(physicalDevice), logicalDevice(logicalDevice),
          graphicsQueue(graphicsQueue), commandPool(commandPool), rendererConfig(rendererConfig)
    {
        imageModel = CreateRef<ImageModel>();

        LoadImage(textureFilePath);
        CreateImageView();
    }

    TextureVulkan::~TextureVulkan()
    {
        VkUtils::DisposeImageModel(logicalDevice, imageModel);
    }

    //todo: make async
    void TextureVulkan::LoadImage(const std::filesystem::path& textureFilePath)
    {
        int texChannels;
        uint64_t bitesPerPixel = 4;

        const auto pixels = stbi_load(textureFilePath.string().c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        const VkDeviceSize imageSize = width * height * bitesPerPixel;

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
        
        imageModel->image = VkUtils::CreateImage(physicalDevice, logicalDevice,
            width, height, rendererConfig->imageFormat,
            tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, imageModel->imageMemory);

        auto commandBuffer = VkUtils::BeginCommandBuffer(logicalDevice, commandPool);

        VkUtils::TransitionImageLayout(logicalDevice, commandBuffer, graphicsQueue, imageModel->image,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);
        VkUtils::SubmitCommandBuffer(graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);

        VkUtils::CopyBufferToImage(logicalDevice, graphicsQueue, stagingBuffer, imageModel->image, width, height, commandPool);

        commandBuffer = VkUtils::BeginCommandBuffer(logicalDevice, commandPool);

        VkUtils::TransitionImageLayout(logicalDevice, commandBuffer, graphicsQueue,imageModel->image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);
        VkUtils::SubmitCommandBuffer(graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);
    }

    void TextureVulkan::CreateImageView()
    {
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VkUtils::CreateImageView(logicalDevice, rendererConfig->imageFormat, aspectFlags, imageModel->image, imageModel->imageView);

    }
}