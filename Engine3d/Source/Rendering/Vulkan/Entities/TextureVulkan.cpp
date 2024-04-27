#include "Pch.h"
#include "TextureVulkan.h"

#include "SharedLib/CustomAssert.h"
#include "Rendering/Vulkan/Buffers/ABuffer.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/Builders/ADescriptorSet.h"
#include "Rendering/Vulkan/Utilities/VkImageViewUtility.h"

#include <stb_image.h>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace AVulkan
{
    TextureVulkan::TextureVulkan(Ref<ProjectSettigns> projectSettings, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
        std::vector<UniformBufferVulkan*>& uniformBuffers, Ref<Descriptors> descriptors, VkDescriptorSetLayout& descriptorSetLayout,
        VkSampler& textureSampler, VkQueue& graphicsQueue, VkCommandPool& commandPool, TextureId textureId)
        : Texture(textureId), physicalDevice(physicalDevice), logicalDevice(logicalDevice), 
                             graphicsQueue(graphicsQueue), commandPool(commandPool)
    {
        this->projectSettings = projectSettings;

        CreateImage(textureId);
        CreateImageView();

        descriptorSet = descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout);
        descriptors->UpdateDescriptorSet(logicalDevice, descriptorSet, descriptorBuffer, imageView, textureSampler);
    }

    TextureVulkan::~TextureVulkan()
    {
        VkImageViewUtility::Destroy(logicalDevice, imageView);
        vkDestroyImage(logicalDevice, image, nullptr);
        vkFreeMemory(logicalDevice, imageMemory, nullptr);
    }

    //todo: make async
    void TextureVulkan::CreateImage(TextureId textureId)
    {
        int width;
        int height;
        int texChannels;
        uint64_t bitesPerPixel = 4;
        VkDeviceSize imageSize;

        auto filePath = projectSettings->projectPath + textures[static_cast<size_t>(textureId)];
        auto pixels = stbi_load(filePath.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        imageSize = width * height * bitesPerPixel;

        if (pixels == nullptr)
        {
            std::filesystem::path fullPath = std::filesystem::absolute(filePath);
            CAssert::Check(pixels != nullptr, "Failed to load texture image: " + fullPath.string());
        }
        else
        {
            spdlog::debug("Image loaded: " + filePath);
        }

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
        
        image = imageUtility.Create(width, height, imgFormat, 
            tiling, usageFlags, memoryFlags, imageMemory);

        imageUtility.TransitionImageLayout(image, imgFormat, 
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        imageUtility.CopyBufferToImage(stagingBuffer, image, width, height);

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
        VkImageViewUtility::Create(logicalDevice, imgFormat, aspectFlags, image, imageView);

    }
}