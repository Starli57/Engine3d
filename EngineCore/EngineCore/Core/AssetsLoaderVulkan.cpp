#include "EngineCore/Pch.h"
#include "AssetsLoaderVulkan.h"

#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <EngineCore/Rendering/Vulkan/Utilities/IndexBufferUtility.h>

#include "EngineCore/Rendering/Vulkan/Utilities/VertexBufferUtility.h"

using namespace AVulkan;


namespace EngineCore
{
    AssetsLoaderVulkan::AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<ResourcesStorageVulkan> assetsDatabase) : 
        AssetsLoader(projectSettings, graphicsApi, assetsDatabase), assetsDatabaseVulkan(assetsDatabase)
    {
        graphicsQueueMutex = CreateUniqueRef<std::mutex>();
    
        vulkanApi = dynamic_cast<GraphicsApiVulkan*>(graphicsApi);

        auto meshesCount = assetsDatabase->meshesIndexByPath.size() + assetsDatabase->customMeshes.size();
        assetsDatabaseVulkan->vertexBuffers.resize(meshesCount);
        assetsDatabaseVulkan->vertexBuffersMemory.resize(meshesCount);
        assetsDatabaseVulkan->indexBuffers.resize(meshesCount);
        assetsDatabaseVulkan->indexBuffersMemory.resize(meshesCount);
        assetsDatabaseVulkan->indexesCount.resize(meshesCount);
        assetsDatabaseVulkan->meshMaterialBinding.resize(meshesCount);
        assetsDatabaseVulkan->boundingBoxMin.resize(meshesCount);
        assetsDatabaseVulkan->boundingBoxMax.resize(meshesCount);
        assetsDatabaseVulkan->boundingBoxCenter.resize(meshesCount);
        
        auto texturesCount = assetsDatabaseVulkan->texturesIndexByPath.size();
        assetsDatabaseVulkan->images.resize(texturesCount);
        assetsDatabaseVulkan->imagesViews.resize(texturesCount);
        assetsDatabaseVulkan->imagesMemory.resize(texturesCount);
    }

    void AssetsLoaderVulkan::LoadTexture(const std::filesystem::path& path)
    {
        auto iter = assetsDatabase->texturesIndexByPath.find(path);
        LoadTexture(path, iter->second);
    }

    void AssetsLoaderVulkan::LoadTexture(const std::filesystem::path& path, const uint32_t textureIndex)
    {
        const auto pathStr = path.string();

        int width;
        int height;
        int channels;

        stbi_uc* texturePixels  = stbi_load(pathStr.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (texturePixels == nullptr) spdlog::error("Failed to load texture image: {}", pathStr);
        else spdlog::info("Image loaded: {}", pathStr);

        uint64_t bitesPerPixel = 4;
        const VkDeviceSize imageSize = width * height * bitesPerPixel;
        uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;
        VkUtils::CreateStagingBuffer(imageSize, texturePixels, stagingBuffer, stagingMemory, vulkanApi->context);
        
        stbi_image_free(texturePixels);
        
        constexpr VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        constexpr VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT  | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        constexpr VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        auto& image = assetsDatabaseVulkan->images.at(textureIndex);
        VkUtils::CreateImage(vulkanApi->context,
            width, height, mipLevels, vulkanApi->context->imageFormat,
            tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, image, assetsDatabaseVulkan->imagesMemory.at(textureIndex));
        
        auto commandPool = vulkanApi->GetCommandPool();
        auto commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->context->logicalDevice, commandPool);

        VkUtils::TransitionImageLayout(commandBuffer, mipLevels, image,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);

        graphicsQueueMutex->lock();
        VkUtils::SubmitCommandBuffer(vulkanApi->context->graphicsQueue, commandBuffer);

        vkFreeCommandBuffers(vulkanApi->context->logicalDevice, commandPool, 1, &commandBuffer);

        VkUtils::CopyBufferToImage(vulkanApi->context, 
            stagingBuffer, image, width, height, commandPool);

        commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->context->logicalDevice, commandPool);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = width;
        int32_t mipHeight = height;

        for (uint32_t i = 1; i < mipLevels; i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                0, 0, nullptr, 0, nullptr, 1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit, VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0, 0, nullptr, 0, nullptr, 1, &barrier);
            
            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }
        
        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);
        
        vkEndCommandBuffer(commandBuffer);

        VkUtils::SubmitCommandBuffer(vulkanApi->context->graphicsQueue, commandBuffer);
        graphicsQueueMutex->unlock();

        vkFreeCommandBuffers(vulkanApi->context->logicalDevice, commandPool, 1, &commandBuffer);

        vkDestroyBuffer(vulkanApi->context->logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(vulkanApi->context->logicalDevice, stagingMemory, nullptr);

        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VkUtils::CreateImageView(vulkanApi->context->logicalDevice, mipLevels, vulkanApi->context->imageFormat, aspectFlags,
            image, assetsDatabaseVulkan->imagesViews.at(textureIndex));

        assetsDatabaseVulkan->textureLoadStatuses.at(textureIndex) = 2;
    }

    void AssetsLoaderVulkan::UnLoadAllTextures()
    {
        for (auto image : assetsDatabaseVulkan->images) VkUtils::DestroyImage(vulkanApi->context->logicalDevice, image);
        for (auto view : assetsDatabaseVulkan->imagesViews) VkUtils::DestroyImageView(vulkanApi->context->logicalDevice, view);
        for (auto imageMemory : assetsDatabaseVulkan->imagesMemory) VkUtils::FreeDeviceMemory(vulkanApi->context->logicalDevice, imageMemory);

        assetsDatabaseVulkan->images.clear();
        assetsDatabaseVulkan->imagesViews.clear();
        assetsDatabaseVulkan->imagesMemory.clear();
    }

    void AssetsLoaderVulkan::LoadMesh(std::filesystem::path& path)
    {
        MeshMeta meshMeta;
        auto meshIter = assetsDatabase->meshesIndexByPath.find(path);
        LoadAndDeserializeMesh(meshIter->first, meshMeta);
        SetupMeshBuffers(meshMeta, meshIter->second);
    }

    void AssetsLoaderVulkan::SetupMeshBuffers(MeshMeta& meshMeta, uint32_t meshIndex)
    {
        auto commandPool = vulkanApi->GetCommandPool();

        graphicsQueueMutex->lock();
        VkUtils::CreateVertexBuffer(vulkanApi->context,
            meshMeta.vertices, assetsDatabaseVulkan->vertexBuffers.at(meshIndex), assetsDatabaseVulkan->vertexBuffersMemory.at(meshIndex), commandPool);

        VkUtils::CreateIndexBuffer(vulkanApi->context, meshMeta.indices, assetsDatabaseVulkan->indexBuffers.at(meshIndex),
            assetsDatabaseVulkan->indexBuffersMemory.at(meshIndex), commandPool);
        graphicsQueueMutex->unlock();

        assetsDatabaseVulkan->indexesCount.at(meshIndex) = static_cast<uint32_t>(meshMeta.indices.size());

        meshMeta.CalculateBoundingBox(
            assetsDatabaseVulkan->boundingBoxMin.at(meshIndex),
            assetsDatabaseVulkan->boundingBoxMax.at(meshIndex),
            assetsDatabaseVulkan->boundingBoxCenter.at(meshIndex));
        
        auto parsedMaterialPath = std::filesystem::path(meshMeta.materialPath);
        auto materialIndex = assetsDatabaseVulkan->materialsIndexByPath.find(parsedMaterialPath);
        if (materialIndex == assetsDatabaseVulkan->materialsIndexByPath.end())
        {
            spdlog::critical("PbrMaterial not found {0}", meshMeta.materialPath);
        }

        assetsDatabaseVulkan->meshMaterialBinding.at(meshIndex) = materialIndex->second;
        assetsDatabaseVulkan->meshLoadStatuses.at(meshIndex) = 2;
    }

    void AssetsLoaderVulkan::UnLoadAllMeshes()
    {
        for (int i = 0; i < assetsDatabaseVulkan->vertexBuffers.size(); i++)
            VkUtils::DisposeVertexBuffer(vulkanApi->context->logicalDevice, assetsDatabaseVulkan->vertexBuffers.at(i), assetsDatabaseVulkan->vertexBuffersMemory.at(i));
        for (int i = 0; i < assetsDatabaseVulkan->indexBuffers.size(); i++)
            VkUtils::DisposeIndexBuffer(vulkanApi->context->logicalDevice, assetsDatabaseVulkan->indexBuffers.at(i), assetsDatabaseVulkan->indexBuffersMemory.at(i));
    }

    void AssetsLoaderVulkan::LoadMaterial(std::filesystem::path& path)
    {
        Ref<PbrMaterial> material = CreateRef<PbrMaterial>();
        auto materialIterator = assetsDatabase->materialsIndexByPath.find(path);
        auto index = materialIterator->second;
        LoadAndDeserializeMaterial(materialIterator->first, material);

        assetsDatabase->materials.at(index) = material;
        assetsDatabase->materialLoadStatuses.at(index) = 2;
    }

    void AssetsLoaderVulkan::UnLoadAllMaterial()
    {
    }
}
