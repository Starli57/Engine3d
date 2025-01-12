#include "EngineCore/Pch.h"
#include "AssetsLoaderVulkan.h"

#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <EngineCore/Rendering/Vulkan/Utilities/IndexBufferUtility.h>

using namespace AVulkan;


AssetsLoaderVulkan::AssetsLoaderVulkan(Ref<ProjectSettings> projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase) : 
    AssetsLoader(projectSettings, graphicsApi, assetsDatabase)
{
}

AssetsLoaderVulkan::~AssetsLoaderVulkan()
{
    UnLoadAllTextures();
    UnLoadAllMeshes();
}

void AssetsLoaderVulkan::LoadAllTextures()
{
    auto vulkanApi = static_cast<AVulkan::GraphicsApiVulkan*>(graphicsApi);
    auto databaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);

    std::vector<int> texturesWidths;
    std::vector<int> texturesHeights;
    std::vector<int> texturesChannels;

    auto texturesCount = databaseVulkan->texturesIndexByPath.size();
    databaseVulkan->images.resize(texturesCount);
    databaseVulkan->imagesViews.resize(texturesCount);
    databaseVulkan->imagesMemory.resize(texturesCount);

    texturesWidths.resize(texturesCount);
    texturesHeights.resize(texturesCount);
    texturesChannels.resize(texturesCount);

    std::vector<stbi_uc*>* texturesPixels = new std::vector<stbi_uc*>();
    texturesPixels->resize(texturesCount);

    std::for_each(std::execution::par, databaseVulkan->texturesIndexByPath.begin(), databaseVulkan->texturesIndexByPath.end(),
        [this, &texturesPixels, &texturesWidths, &texturesHeights, &texturesChannels](const auto& pair)
    {
        auto pathStr = pair.first.string();
        auto textureIndex = pair.second;

        int width;
        int height;
        int channels;

        texturesPixels->at(textureIndex) = stbi_load(pathStr.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        texturesWidths.at(textureIndex) = width;
        texturesHeights.at(textureIndex) = height;
        texturesChannels.at(textureIndex) = channels;

        if (texturesPixels->at(textureIndex) == nullptr)
        {
            spdlog::error("Failed to load texture image: {}", pathStr);
        }
        else
        {
            spdlog::info("Image loaded: {}", pathStr);
        }
    });

    //todo: make parallel
    for(int textureIndex = 0; textureIndex < databaseVulkan->texturesPaths.size(); textureIndex++)
    {
        uint64_t bitesPerPixel = 4;
        int width = texturesWidths.at(textureIndex);
        int height = texturesHeights.at(textureIndex);
        const VkDeviceSize imageSize = texturesWidths.at(textureIndex) * texturesHeights.at(textureIndex) * bitesPerPixel;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        VkBufferUsageFlags usageFlagsStaging = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags memoryFlagsStaging = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VkUtils::CreateBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice, imageSize,
            usageFlagsStaging, memoryFlagsStaging, stagingBuffer, stagingMemory);

        void* data;
        vkMapMemory(vulkanApi->logicalDevice, stagingMemory, 0, imageSize, 0, &data);
        memcpy(data, texturesPixels->at(textureIndex), static_cast<size_t>(imageSize));
        vkUnmapMemory(vulkanApi->logicalDevice, stagingMemory);

        stbi_image_free(texturesPixels->at(textureIndex));

        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        databaseVulkan->images.at(textureIndex) = VkUtils::CreateImage(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
            width, height, vulkanApi->rendererConfig->imageFormat,
            tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, databaseVulkan->imagesMemory.at(textureIndex));

        auto commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->logicalDevice, vulkanApi->commandPool);

        VkUtils::TransitionImageLayout(vulkanApi->logicalDevice, commandBuffer, vulkanApi->graphicsQueue, databaseVulkan->images.at(textureIndex),
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);
        VkUtils::SubmitCommandBuffer(vulkanApi->graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(vulkanApi->logicalDevice, vulkanApi->commandPool, 1, &commandBuffer);

        VkUtils::CopyBufferToImage(vulkanApi->logicalDevice, vulkanApi->graphicsQueue, 
            stagingBuffer, databaseVulkan->images.at(textureIndex), width, height, vulkanApi->commandPool);

        commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->logicalDevice, vulkanApi->commandPool);

        VkUtils::TransitionImageLayout(vulkanApi->logicalDevice, commandBuffer, vulkanApi->graphicsQueue, databaseVulkan->images.at(textureIndex),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);
        VkUtils::SubmitCommandBuffer(vulkanApi->graphicsQueue, commandBuffer);
        vkFreeCommandBuffers(vulkanApi->logicalDevice, vulkanApi->commandPool, 1, &commandBuffer);

        vkDestroyBuffer(vulkanApi->logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(vulkanApi->logicalDevice, stagingMemory, nullptr);

        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VkUtils::CreateImageView(vulkanApi->logicalDevice, vulkanApi->rendererConfig->imageFormat, aspectFlags,
            databaseVulkan->images.at(textureIndex), databaseVulkan->imagesViews.at(textureIndex));
    }

    delete texturesPixels;
}

void AssetsLoaderVulkan::UnLoadAllTextures()
{
    auto vulkanApi = static_cast<AVulkan::GraphicsApiVulkan*>(graphicsApi);
    auto databaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);

    for (auto image : databaseVulkan->images) VkUtils::DestroyImage(vulkanApi->logicalDevice, image);
    for (auto view : databaseVulkan->imagesViews) VkUtils::DestroyImageView(vulkanApi->logicalDevice, view);
    for (auto imageMemory : databaseVulkan->imagesMemory) VkUtils::FreeDeviceMemory(vulkanApi->logicalDevice, imageMemory);

    databaseVulkan->images.clear();
    databaseVulkan->imagesViews.clear();
    databaseVulkan->imagesMemory.clear();
}

void AssetsLoaderVulkan::LoadAllMeshes(std::vector<MeshMeta>& meshes)
{
    auto vulkanApi = dynamic_cast<AVulkan::GraphicsApiVulkan*>(graphicsApi);
    auto databaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);
    auto meshesCount = meshes.size();

    databaseVulkan->vertexBuffers.resize(meshesCount);
    databaseVulkan->vertexBuffersMemory.resize(meshesCount);
    databaseVulkan->indexBuffers.resize(meshesCount);
    databaseVulkan->indexBuffersMemory.resize(meshesCount);
    databaseVulkan->indexesCount.resize(meshesCount);
    databaseVulkan->meshMaterialBinding.resize(meshesCount);
    
    for (int i = 0; i < meshesCount; i++)
    {
        VkUtils::CreateVertexBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
            meshes.at(i).vertices, databaseVulkan->vertexBuffers.at(i), databaseVulkan->vertexBuffersMemory.at(i), 
            vulkanApi->graphicsQueue, vulkanApi->commandPool);

        VkUtils::CreateIndexBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
            meshes.at(i).indices, databaseVulkan->indexBuffers.at(i), databaseVulkan->indexBuffersMemory.at(i), 
            vulkanApi->graphicsQueue, vulkanApi->commandPool);

        databaseVulkan->indexesCount.at(i) = static_cast<uint32_t>(meshes.at(i).indices.size());

        auto parsedMaterialPath = std::filesystem::path(meshes.at(i).materialPath);
        auto materialIndex = databaseVulkan->materialsIndexByPath.find(parsedMaterialPath);
        if (materialIndex == databaseVulkan->materialsIndexByPath.end())
        {
            spdlog::critical("Material not found {0}", meshes.at(i).materialPath);
        }

        databaseVulkan->meshMaterialBinding.at(i) = materialIndex->second;
    }
}

void AssetsLoaderVulkan::UnLoadAllMeshes()
{
    auto vulkanApi = dynamic_cast<AVulkan::GraphicsApiVulkan*>(graphicsApi);
    auto databaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);
    for (int i = 0; i < databaseVulkan->vertexBuffers.size(); i++)
        VkUtils::DisposeVertexBuffer(vulkanApi->logicalDevice, databaseVulkan->vertexBuffers.at(i), databaseVulkan->vertexBuffersMemory.at(i));
    for (int i = 0; i < databaseVulkan->indexBuffers.size(); i++)
        VkUtils::DisposeIndexBuffer(vulkanApi->logicalDevice, databaseVulkan->indexBuffers.at(i), databaseVulkan->indexBuffersMemory.at(i));
}
