#include "Pch.h"
#include "TextureVulkan.h"

#include "EngineShared/CustomAssert.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Rendering/Vulkan/Utilities/BufferUtility.h"
#include "Rendering/Vulkan/Builders/AUniformBufferVulkan.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/Builders/AImageView.h"

#include <stb_image.h>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace AVulkan
{
    TextureVulkan::TextureVulkan(Ref<ProjectSettigns> projectSettings, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
        Ref<Descriptors> descriptors, VkDescriptorSetLayout& descriptorSetLayout,
        VkSampler& textureSampler, VkQueue& graphicsQueue, VkCommandPool& commandPool, std::filesystem::path& textureFilePath, Ref<Rollback> rollback)
        : Texture(textureFilePath), physicalDevice(physicalDevice), logicalDevice(logicalDevice), descriptors(descriptors),
                     graphicsQueue(graphicsQueue), commandPool(commandPool), textureSampler(textureSampler), rollback(rollback)
    {
        this->projectSettings = projectSettings;
        auto uniformBufferBuilder = AUniformBufferVulkan();

        descriptorSets = std::vector<VkDescriptorSet>();
        uboViewProjection = std::vector<Ref<BufferModel>>();
        uboLights = std::vector<Ref<BufferModel>>();

        imageModel = CreateRef<ImageModel>();

        CreateImage(textureFilePath, rollback);
        CreateImageView();

        for (uint16_t i = 0; i < VulkanGraphicsApi::maxFramesInFlight; i++)
        {
            auto viewProjectionDescriptorSet = descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, rollback);
            descriptorSets.push_back(viewProjectionDescriptorSet);

            auto viewProjection = uniformBufferBuilder.Create(logicalDevice, physicalDevice, sizeof(UboViewProjectionComponent), rollback);
            uboViewProjection.push_back(viewProjection);

            auto lightningDescriptorSet = descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, rollback);
            descriptorSets.push_back(lightningDescriptorSet);

            auto lightning = uniformBufferBuilder.Create(logicalDevice, physicalDevice, sizeof(UboDiffuseLightComponent), rollback);
            uboLights.push_back(lightning);
        }

        rollback->Add([this]()
        {
            uboViewProjection.clear();
            uboLights.clear();
            descriptorSets.clear();
        });
    }

    TextureVulkan::~TextureVulkan()
    {
    }

    void TextureVulkan::UpdateDescriptors(uint16_t frame)
    {
        descriptors->UpdateDescriptorSet(
                logicalDevice, descriptorSets[frame],
                uboViewProjection[frame]->buffer, sizeof(UboViewProjectionComponent),
                uboLights[frame]->buffer, sizeof(UboDiffuseLightComponent),
                imageModel->imageView, textureSampler);
    }

    //todo: make async
    void TextureVulkan::CreateImage(std::filesystem::path& textureFilePath, Ref<Rollback> rollback)
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

        //todo: find suitable format firstly
        VkFormat imgFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        
        AImage imageUtility(physicalDevice, logicalDevice, graphicsQueue, commandPool);
        
        imageModel->image = imageUtility.Create(width, height, imgFormat,
            tiling, usageFlags, memoryFlags, imageModel->imageMemory);

        imageUtility.TransitionImageLayout(imageModel->image, imgFormat,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        imageUtility.CopyBufferToImage(stagingBuffer, imageModel->image, width, height);

        imageUtility.TransitionImageLayout(imageModel->image, imgFormat,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice, stagingMemory, nullptr);

        rollback->Add([this]() { vkDestroyImage(logicalDevice, imageModel->image, nullptr); });
        rollback->Add([this]() { vkFreeMemory(logicalDevice, imageModel->imageMemory, nullptr); });
    }

    void TextureVulkan::CreateImageView()
    {
        //todo: find suitable format firstly
        VkFormat imgFormat = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        AImageView().Create(logicalDevice, imgFormat, aspectFlags, imageModel->image, imageModel->imageView, rollback);

    }
}