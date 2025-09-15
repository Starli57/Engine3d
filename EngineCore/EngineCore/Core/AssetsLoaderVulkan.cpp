#include "EngineCore/Pch.h"
#include "AssetsLoaderVulkan.h"

#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <stb_image.h>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include "EngineCore/Utilities/YamlConverters.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Utilities/VertexBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/IndexBufferUtility.h"

namespace Engine
{
    AssetsLoaderVulkan::AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<ResourcesStorageVulkan> assetsDatabase) : 
        projectSettings(projectSettings), assetsDatabase(assetsDatabase)
    {
        graphicsQueueMutex = CreateUniqueRef<std::mutex>();
    
        vulkanApi = dynamic_cast<VulkanApi::GraphicsApiVulkan*>(graphicsApi);
    	assetsDatabase->materialLoadStatuses.resize(assetsDatabase->materialsPaths.size());
    	assetsDatabase->materials.resize(assetsDatabase->materialsPaths.size());

        auto meshesCount = assetsDatabase->meshesIndexByPath.size() + assetsDatabase->customMeshes.size();
    	assetsDatabase->meshLoadStatuses.resize(meshesCount);
        assetsDatabase->vertexBuffers.resize(meshesCount);
        assetsDatabase->vertexBuffersMemory.resize(meshesCount);
        assetsDatabase->indexBuffers.resize(meshesCount);
        assetsDatabase->indexBuffersMemory.resize(meshesCount);
        assetsDatabase->indexesCount.resize(meshesCount);
        assetsDatabase->meshMaterialBinding.resize(meshesCount);
        assetsDatabase->boundingBoxMin.resize(meshesCount);
        assetsDatabase->boundingBoxMax.resize(meshesCount);
        assetsDatabase->boundingBoxCenter.resize(meshesCount);
        
        auto texturesCount = assetsDatabase->texturesIndexByPath.size();
    	assetsDatabase->textureLoadStatuses.resize(texturesCount);
        assetsDatabase->images.resize(texturesCount);
        assetsDatabase->imagesViews.resize(texturesCount);
        assetsDatabase->imagesMemory.resize(texturesCount);
    	
    	//SetupMeshBuffers(cubeDefinition);
    	//CreateMesh(sphereDefiniton);
    }

	void AssetsLoaderVulkan::Load()
	{
		LoadRequestedMaterials();
		LoadRequestedMeshes();
	}

	uint32_t AssetsLoaderVulkan::GetTextureStr(const std::string& path) const
	{
		return GetTexture(std::filesystem::path(path));
	}

	uint32_t AssetsLoaderVulkan::GetTexture(const std::filesystem::path& path) const
	{
		assetsDatabase->AddTextureLoadRequest(path);
		auto textureIter = assetsDatabase->texturesIndexByPath.find(path);
		CAssert::Check(textureIter != assetsDatabase->texturesIndexByPath.end(), "Texture file not found " + path.string());
		return textureIter->second;
	}

	uint32_t AssetsLoaderVulkan::GetOrLoadTextureStr(const std::string& pathStr)
	{
		auto path = std::filesystem::path(pathStr);
		return GetOrLoadTexture(path);
	}

	uint32_t AssetsLoaderVulkan::GetOrLoadTexture(const std::filesystem::path& path)
	{
		auto textureIter = assetsDatabase->texturesIndexByPath.find(path);
		if (!assetsDatabase->SetTextureLoadingStatus(path)) return textureIter->second;

		LoadTexture(path);
		return textureIter->second;
	}

	void AssetsLoaderVulkan::LoadAndDeserializeMesh(const std::filesystem::path& path, MeshAsset& meshAsset) const
	{
		std::ifstream inFile(path.string(), std::ios::binary);
		if (!inFile)
		{
			spdlog::critical("Failed to open mesh file for reading by path={}", path.string());
			return;
		}

		size_t vertexCount;
		inFile.read(reinterpret_cast<char*>(&vertexCount), sizeof(size_t));
		meshAsset.vertices.resize(vertexCount);
		inFile.read(reinterpret_cast<char*>(meshAsset.vertices.data()), vertexCount * sizeof(Vertex));

		size_t indexCount;
		inFile.read(reinterpret_cast<char*>(&indexCount), sizeof(size_t));
		meshAsset.indices.resize(indexCount);
		inFile.read(reinterpret_cast<char*>(meshAsset.indices.data()), indexCount * sizeof(uint32_t));

		size_t materialPathSize;
		inFile.read(reinterpret_cast<char*>(&materialPathSize), sizeof(materialPathSize));
		meshAsset.materialPath.resize(materialPathSize);
		inFile.read(reinterpret_cast<char*>(&meshAsset.materialPath[0]), materialPathSize);

		size_t materialNameSize;
		inFile.read(reinterpret_cast<char*>(&materialNameSize), sizeof(materialNameSize));
		meshAsset.materialName.resize(materialNameSize);
		inFile.read(reinterpret_cast<char*>(&meshAsset.materialName[0]), materialNameSize);
		
		inFile.close();
	}

	void AssetsLoaderVulkan::LoadAndDeserializeMaterial(const std::filesystem::path& path, const Ref<PbrMaterialAsset>& material)
	{
		std::vector<YAML::Node> data;

		try
		{
			data = YAML::LoadAllFromFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			spdlog::critical("Failed to load material by path={0} error={1}", path.string(), e.what());
			return;
		}

		if (data.size() > 1) spdlog::warn("PbrMaterial file has more than 1 material at path={}", path.string());

		auto node = data[0];
		material->pipelineId = node["pipelineName"].as<std::string>();
		material->opaque = node["isOpaque"].as<bool>();

		material->baseColor = node["baseColor"].as<glm::vec4>();
		material->roughness = node["roughness"].as<float>();
		material->metallic = node["metallic"].as<float>();
		material->alphaCutoff = node["alphaCutoffFactor"].as<float>();
		material->normalsScale = node["normalsScale"] ? node["normalsScale"].as<float>() : 1.0f;
    	
		auto baseColorTexturePath = node["baseColorTextureName"] ? node["baseColorTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto diffuseTextureIndex = GetOrLoadTextureStr(baseColorTexturePath);
		material->baseTexture = diffuseTextureIndex;

		auto normalTexturePath = node["normalsTextureName"] ? node["normalsTextureName"].as<std::string>() : projectSettings->resourcesPath + "/black_box.png";
		auto normalTextureIndex = GetOrLoadTextureStr(normalTexturePath);
		material->normalsTexture = normalTextureIndex;

		auto metallicRoughnessPath = node["metallicRoughnessTextureName"] ? node["metallicRoughnessTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto metallicRoughnessTextureIndex = GetOrLoadTextureStr(metallicRoughnessPath);
		material->metallicRoughnessTexture = metallicRoughnessTextureIndex;
		
		auto occlusionTexturePath = node["occlusionTextureName"] ? node["occlusionTextureName"].as<std::string>() : projectSettings->resourcesPath + "/white_box.png";
		auto occlusionTextureIndex = GetOrLoadTextureStr(occlusionTexturePath);
		material->occlusionTexture = occlusionTextureIndex;
		
		auto emissiveTexturePath = node["emissiveTextureName"] ? node["emissiveTextureName"].as<std::string>() : projectSettings->resourcesPath + "/black_box.png";
		auto emissiveTextureIndex = GetOrLoadTextureStr(emissiveTexturePath);
		material->emissiveTexture = emissiveTextureIndex;
	}

	void AssetsLoaderVulkan::LoadRequestedMeshes()
	{
		std::vector<uint32_t> loadMeshes;
		assetsDatabase->meshLoadStatusMutex.lock();
		{
			loadMeshes.resize(assetsDatabase->meshLoadRequests.size());
		
			for (uint32_t i = 0; i < loadMeshes.size(); i++)
			{
				loadMeshes[i] = assetsDatabase->meshLoadRequests.front();
				assetsDatabase->meshLoadRequests.pop();
			}
		}
		assetsDatabase->meshLoadStatusMutex.unlock();
		if (loadMeshes.empty()) return;
	
		std::for_each(std::execution::par, loadMeshes.begin(), loadMeshes.end(), [this](const auto& meshIndex)
			{
				auto path = assetsDatabase->meshesPaths[meshIndex];
				LoadMesh(path);
			});
	}

	void AssetsLoaderVulkan::LoadRequestedMaterials()
	{
		std::vector<uint32_t> loadMaterials;
		assetsDatabase->materialLoadStatusMutex.lock();
		{
			loadMaterials.resize(assetsDatabase->materialLoadRequests.size());
	
			for (uint32_t i = 0; i < loadMaterials.size(); i++)
			{
				loadMaterials[i] = assetsDatabase->materialLoadRequests.front();
				assetsDatabase->materialLoadRequests.pop();
			}
		}
		assetsDatabase->materialLoadStatusMutex.unlock();
		if (loadMaterials.empty()) return;
	
		std::for_each(std::execution::par, loadMaterials.begin(), loadMaterials.end(), [this](const auto& materialIndex)
			{
				auto path = assetsDatabase->materialsPaths[materialIndex];
				LoadMaterial(path);
			});
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
        VulkanApi::CreateStagingBuffer(vulkanApi->vulkanContext, imageSize, texturePixels, stagingBuffer, stagingMemory);
        
        stbi_image_free(texturePixels);
        
        constexpr VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        constexpr VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT  | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        constexpr VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        auto& image = assetsDatabase->images.at(textureIndex);
        VulkanApi::CreateImage(vulkanApi->vulkanContext,
            width, height, mipLevels, vulkanApi->vulkanContext->imageFormat,
            tiling, usageFlags, VK_SAMPLE_COUNT_1_BIT, memoryFlags, image, assetsDatabase->imagesMemory.at(textureIndex));
        
        auto commandPool = vulkanApi->GetCommandPool();
        auto commandBuffer = VulkanApi::BeginCommandBuffer(vulkanApi->vulkanContext->logicalDevice, commandPool);

        VulkanApi::TransitionImageLayout(commandBuffer, mipLevels, image,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

        vkEndCommandBuffer(commandBuffer);

        graphicsQueueMutex->lock();
        VulkanApi::SubmitCommandBuffer(vulkanApi->vulkanContext->graphicsQueue, commandBuffer);

        vkFreeCommandBuffers(vulkanApi->vulkanContext->logicalDevice, commandPool, 1, &commandBuffer);

        VulkanApi::CopyBufferToImage(vulkanApi->vulkanContext, 
            stagingBuffer, image, width, height, commandPool);

        commandBuffer = VulkanApi::BeginCommandBuffer(vulkanApi->vulkanContext->logicalDevice, commandPool);

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

        VulkanApi::SubmitCommandBuffer(vulkanApi->vulkanContext->graphicsQueue, commandBuffer);
        graphicsQueueMutex->unlock();

        vkFreeCommandBuffers(vulkanApi->vulkanContext->logicalDevice, commandPool, 1, &commandBuffer);

        vkDestroyBuffer(vulkanApi->vulkanContext->logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(vulkanApi->vulkanContext->logicalDevice, stagingMemory, nullptr);

        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VulkanApi::CreateImageView(vulkanApi->vulkanContext->logicalDevice, mipLevels, vulkanApi->vulkanContext->imageFormat, aspectFlags,
            image, assetsDatabase->imagesViews.at(textureIndex));

        assetsDatabase->textureLoadStatuses.at(textureIndex) = 2;
    }

    void AssetsLoaderVulkan::UnLoadAllTextures()
    {
        for (auto image : assetsDatabase->images) VulkanApi::DestroyImage(vulkanApi->vulkanContext->logicalDevice, image);
        for (auto view : assetsDatabase->imagesViews) VulkanApi::DestroyImageView(vulkanApi->vulkanContext->logicalDevice, view);
        for (auto imageMemory : assetsDatabase->imagesMemory) VulkanApi::FreeDeviceMemory(vulkanApi->vulkanContext->logicalDevice, imageMemory);

        assetsDatabase->images.clear();
        assetsDatabase->imagesViews.clear();
        assetsDatabase->imagesMemory.clear();
    }

    void AssetsLoaderVulkan::LoadMesh(std::filesystem::path& path)
    {
        MeshAsset meshAsset;
        auto meshIter = assetsDatabase->meshesIndexByPath.find(path);
        LoadAndDeserializeMesh(meshIter->first, meshAsset);
        SetupMeshBuffers(meshAsset, meshIter->second);
    }

    void AssetsLoaderVulkan::SetupMeshBuffers(MeshAsset& meshAsset, uint32_t meshIndex)
    {
        auto commandPool = vulkanApi->GetCommandPool();

        graphicsQueueMutex->lock();
        VulkanApi::CreateVertexBuffer(vulkanApi->vulkanContext,
            meshAsset.vertices, assetsDatabase->vertexBuffers.at(meshIndex), assetsDatabase->vertexBuffersMemory.at(meshIndex), commandPool);

        VulkanApi::CreateIndexBuffer(vulkanApi->vulkanContext, meshAsset.indices, assetsDatabase->indexBuffers.at(meshIndex),
            assetsDatabase->indexBuffersMemory.at(meshIndex), commandPool);
        graphicsQueueMutex->unlock();

        assetsDatabase->indexesCount.at(meshIndex) = static_cast<uint32_t>(meshAsset.indices.size());

        meshAsset.CalculateBoundingBox(
            assetsDatabase->boundingBoxMin.at(meshIndex),
            assetsDatabase->boundingBoxMax.at(meshIndex),
            assetsDatabase->boundingBoxCenter.at(meshIndex));
        
        auto parsedMaterialPath = std::filesystem::path(meshAsset.materialPath);
        auto materialIndex = assetsDatabase->materialsIndexByPath.find(parsedMaterialPath);
        if (materialIndex == assetsDatabase->materialsIndexByPath.end())
        {
            spdlog::critical("PbrMaterial not found {0}", meshAsset.materialPath);
        }

        assetsDatabase->meshMaterialBinding.at(meshIndex) = materialIndex->second;
        assetsDatabase->meshLoadStatuses.at(meshIndex) = 2;
    }

    void AssetsLoaderVulkan::UnLoadAllMeshes()
    {
        for (int i = 0; i < assetsDatabase->vertexBuffers.size(); i++)
            VulkanApi::DisposeVertexBuffer(vulkanApi->vulkanContext->logicalDevice, assetsDatabase->vertexBuffers.at(i), assetsDatabase->vertexBuffersMemory.at(i));
        for (int i = 0; i < assetsDatabase->indexBuffers.size(); i++)
            VulkanApi::DisposeIndexBuffer(vulkanApi->vulkanContext->logicalDevice, assetsDatabase->indexBuffers.at(i), assetsDatabase->indexBuffersMemory.at(i));
    }

    void AssetsLoaderVulkan::LoadMaterial(std::filesystem::path& path)
    {
        Ref<PbrMaterialAsset> material = CreateRef<PbrMaterialAsset>();
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
