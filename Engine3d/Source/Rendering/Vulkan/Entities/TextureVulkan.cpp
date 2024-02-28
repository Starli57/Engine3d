#include "Pch.h"
#include "TextureVulkan.h"

#include "Architecture/CustomAssert.h"
#include "Rendering/Vulkan/Buffers/ABuffer.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/AssetsDatabaseVulkan.h"

namespace AVulkan
{
    TextureVulkan::TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
        VkQueue& graphicsQueue, VkCommandPool& commandPool, AssetsDatabaseVulkan& assetsDatabase,
        const std::string& filePath, int& width, int& height) : Texture(filePath, width, height), path(filePath), logicalDevice(logicalDevice)
    {
        Create(physicalDevice, graphicsQueue, commandPool, assetsDatabase, filePath, width, height);
    }

    TextureVulkan::~TextureVulkan()
    {
        vkDestroyImage(logicalDevice, *texture.get(), nullptr);
        vkFreeMemory(logicalDevice, *textureMemory.get(), nullptr);
    }

    //todo: make async
    Ref<stbi_uc> TextureVulkan::Load(const std::string& filePath, int& width, int& height, VkDeviceSize* imageSize)
    {
        uint64_t bitesPerPixel = 4;
        auto pixels = IOUtility().LoadTexture(filePath, &width, &height, 0, STBI_rgb_alpha);
        VkDeviceSize imageSize = width * height * bitesPerPixel;

        CAssert::Check(pixels.get() != nullptr, "Failed to load texture image: " + filePath);

        return pixels;
    }

    //todo: make async
    void TextureVulkan::Create(VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool,
        AssetsDatabaseVulkan& assetsDatabase, const std::string& filePath, int& width, int& height)
    {
        VkDeviceSize imageSize;
        auto textureData = Load(filePath, width, height, &imageSize);

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        ABuffer().Create(physicalDevice, logicalDevice, imageSize,
            usageFlags, memoryFlags, stagingBuffer, stagingMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingMemory, 0, imageSize, 0, &data);
        memcpy(data, textureData.get(), static_cast<size_t>(imageSize));
        vkUnmapMemory(logicalDevice, stagingMemory);

        stbi_image_free(textureData.get());

        //todo: find suitable format firstly
        VkFormat imgFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        
        AImage imageUtility;
        imageUtility.Create(physicalDevice, logicalDevice, width, height, imgFormat, tiling, usageFlags, memoryFlags, *textureMemory.get());
        imageUtility.CopyBufferToImage(logicalDevice, graphicsQueue, commandPool, stagingBuffer, *texture.get(), width, height);
        assetsDatabase.AddTexture(CreateRef<TextureVulkan>(this));

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);
    }
}