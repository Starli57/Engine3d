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


AssetsLoaderVulkan::AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase) : 
    AssetsLoader(projectSettings, graphicsApi, assetsDatabase)
{
    graphicsQueueMutex = CreateUniqueRef<std::mutex>();
    
    vulkanApi = dynamic_cast<GraphicsApiVulkan*>(graphicsApi);
    assetsDatabaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);

    auto meshesCount = assetsDatabase->meshesIndexByPath.size() + assetsDatabase->customMeshes.size();
    assetsDatabaseVulkan->vertexBuffers.resize(meshesCount);
    assetsDatabaseVulkan->vertexBuffersMemory.resize(meshesCount);
    assetsDatabaseVulkan->indexBuffers.resize(meshesCount);
    assetsDatabaseVulkan->indexBuffersMemory.resize(meshesCount);
    assetsDatabaseVulkan->indexesCount.resize(meshesCount);
    assetsDatabaseVulkan->meshMaterialBinding.resize(meshesCount);

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

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;

    VkBufferUsageFlags usageFlagsStaging = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    constexpr VkMemoryPropertyFlags memoryFlagsStaging = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkUtils::CreateBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice, imageSize,
        usageFlagsStaging, memoryFlagsStaging, stagingBuffer, stagingMemory);

    void* data;
    vkMapMemory(vulkanApi->logicalDevice, stagingMemory, 0, imageSize, 0, &data);
    memcpy(data, texturePixels, imageSize);
    vkUnmapMemory(vulkanApi->logicalDevice, stagingMemory);

    stbi_image_free(texturePixels);
        
    constexpr VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    constexpr VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    constexpr VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    assetsDatabaseVulkan->images.at(textureIndex) = VkUtils::CreateImage(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
        width, height, vulkanApi->rendererConfig->imageFormat,
        tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, assetsDatabaseVulkan->imagesMemory.at(textureIndex));

    auto commandPool = vulkanApi->GetCommandPool();
    auto commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->logicalDevice, commandPool);

    VkUtils::TransitionImageLayout(commandBuffer, assetsDatabaseVulkan->images.at(textureIndex),
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

    vkEndCommandBuffer(commandBuffer);

    graphicsQueueMutex->lock();
    VkUtils::SubmitCommandBuffer(vulkanApi->graphicsQueue, commandBuffer);

    vkFreeCommandBuffers(vulkanApi->logicalDevice, commandPool, 1, &commandBuffer);

    VkUtils::CopyBufferToImage(vulkanApi->logicalDevice, vulkanApi->graphicsQueue, 
        stagingBuffer, assetsDatabaseVulkan->images.at(textureIndex), width, height, commandPool);

    commandBuffer = VkUtils::BeginCommandBuffer(vulkanApi->logicalDevice, commandPool);

    VkUtils::TransitionImageLayout(commandBuffer, assetsDatabaseVulkan->images.at(textureIndex), 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

    vkEndCommandBuffer(commandBuffer);

    VkUtils::SubmitCommandBuffer(vulkanApi->graphicsQueue, commandBuffer);
    graphicsQueueMutex->unlock();

    vkFreeCommandBuffers(vulkanApi->logicalDevice, commandPool, 1, &commandBuffer);

    vkDestroyBuffer(vulkanApi->logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(vulkanApi->logicalDevice, stagingMemory, nullptr);

    VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    VkUtils::CreateImageView(vulkanApi->logicalDevice, vulkanApi->rendererConfig->imageFormat, aspectFlags,
        assetsDatabaseVulkan->images.at(textureIndex), assetsDatabaseVulkan->imagesViews.at(textureIndex));

    assetsDatabaseVulkan->textureLoadStatuses.at(textureIndex) = 2;
}

void AssetsLoaderVulkan::LoadAllTextures()
{
    std::for_each(std::execution::par, assetsDatabaseVulkan->texturesIndexByPath.begin(), assetsDatabaseVulkan->texturesIndexByPath.end(),
        [this](const auto& pair)
        {
            auto path = pair.first;
            LoadTexture(path, pair.second);
        });
}

void AssetsLoaderVulkan::UnLoadAllTextures()
{
    for (auto image : assetsDatabaseVulkan->images) VkUtils::DestroyImage(vulkanApi->logicalDevice, image);
    for (auto view : assetsDatabaseVulkan->imagesViews) VkUtils::DestroyImageView(vulkanApi->logicalDevice, view);
    for (auto imageMemory : assetsDatabaseVulkan->imagesMemory) VkUtils::FreeDeviceMemory(vulkanApi->logicalDevice, imageMemory);

    assetsDatabaseVulkan->images.clear();
    assetsDatabaseVulkan->imagesViews.clear();
    assetsDatabaseVulkan->imagesMemory.clear();
}

void AssetsLoaderVulkan::LoadMesh(std::filesystem::path& path)
{
    MeshMeta meshMeta;
    auto meshIter = assetsDatabase->meshesIndexByPath.find(path);
    DeserializeMeshMeta(meshIter->first, meshMeta);
    SetupMesh(meshMeta, meshIter->second);
}

void AssetsLoaderVulkan::SetupMesh(MeshMeta& meshMeta, uint32_t meshIndex)
{
    auto commandPool = vulkanApi->GetCommandPool();

    graphicsQueueMutex->lock();
    VkUtils::CreateVertexBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
        meshMeta.vertices, assetsDatabaseVulkan->vertexBuffers.at(meshIndex), assetsDatabaseVulkan->vertexBuffersMemory.at(meshIndex),
        vulkanApi->graphicsQueue, commandPool);

    VkUtils::CreateIndexBuffer(vulkanApi->physicalDevice, vulkanApi->logicalDevice,
        meshMeta.indices, assetsDatabaseVulkan->indexBuffers.at(meshIndex), assetsDatabaseVulkan->indexBuffersMemory.at(meshIndex),
        vulkanApi->graphicsQueue, commandPool);
    graphicsQueueMutex->unlock();

    assetsDatabaseVulkan->indexesCount.at(meshIndex) = static_cast<uint32_t>(meshMeta.indices.size());

    auto parsedMaterialPath = std::filesystem::path(meshMeta.materialPath);
    auto materialIndex = assetsDatabaseVulkan->materialsIndexByPath.find(parsedMaterialPath);
    if (materialIndex == assetsDatabaseVulkan->materialsIndexByPath.end())
    {
        spdlog::critical("Material not found {0}", meshMeta.materialPath);
    }

    assetsDatabaseVulkan->meshMaterialBinding.at(meshIndex) = materialIndex->second;
    assetsDatabaseVulkan->meshLoadStatuses.at(meshIndex) = 2;
}

void AssetsLoaderVulkan::LoadAllMeshes()
{
    std::for_each(std::execution::par, assetsDatabase->meshesIndexByPath.begin(), assetsDatabase->meshesIndexByPath.end(), [this](const auto& pair)
    {
        MeshMeta meshMeta;
        DeserializeMeshMeta(pair.first, meshMeta);
        SetupMesh(meshMeta, pair.second);
    });
}

void AssetsLoaderVulkan::UnLoadAllMeshes()
{
    for (int i = 0; i < assetsDatabaseVulkan->vertexBuffers.size(); i++)
        VkUtils::DisposeVertexBuffer(vulkanApi->logicalDevice, assetsDatabaseVulkan->vertexBuffers.at(i), assetsDatabaseVulkan->vertexBuffersMemory.at(i));
    for (int i = 0; i < assetsDatabaseVulkan->indexBuffers.size(); i++)
        VkUtils::DisposeIndexBuffer(vulkanApi->logicalDevice, assetsDatabaseVulkan->indexBuffers.at(i), assetsDatabaseVulkan->indexBuffersMemory.at(i));
}
