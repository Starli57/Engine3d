#include "Pch.h"
#include "TextureVulkan.h"

#include "Architecture/CustomAssert.h"
#include "Rendering/Vulkan/Buffers/ABuffer.h"
#include "Rendering/Vulkan/Builders/AImage.h"

namespace AVulkan
{
    TextureVulkan::TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
        VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& filePath) 
        : Texture(filePath), logicalDevice(logicalDevice)
    {
        Create(physicalDevice, graphicsQueue, commandPool, filePath);
    }

    TextureVulkan::~TextureVulkan()
    {
        vkDestroyImage(logicalDevice, *texture.get(), nullptr);
        vkFreeMemory(logicalDevice, *textureMemory.get(), nullptr);
    }

    //todo: make async
    void TextureVulkan::Create(VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::string& filePath)
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
        
        texture = CreateRef<VkImage>(imageUtility.Create(width, height, imgFormat, 
            tiling, usageFlags, memoryFlags, *textureMemory.get()));

        imageUtility.TransitionImageLayout(texture, imgFormat, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        imageUtility.CopyBufferToImage(stagingBuffer, *texture.get(), width, height);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);
    }
}