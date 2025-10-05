#include "EngineCore/Pch.h"
#include "DescriptorMaterialOpaque.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboMaterial.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkBufferWrapper.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/DescriptorsAllocator.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/UniformBufferVulkanUtility.h"

namespace ClientVulkanApi
{
    #pragma optimize("", off)
    DescriptorMaterialOpaque::DescriptorMaterialOpaque(Engine::EngineContext* engineContext)
        : engineContext(engineContext), vulkanContext(engineContext->renderer->vulkanContext)
    {
        descriptorPools.resize(VulkanApi::VulkanContext::maxFramesInFlight);
        for(int i = 0; i < VulkanApi::VulkanContext::maxFramesInFlight; i++)
        {
            VulkanApi::CreateDescriptorPool(vulkanContext->logicalDevice, descriptorPools.at(i), VulkanApi::maxDescriptorSets);
        }
        
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorMaterialOpaque::~DescriptorMaterialOpaque()
    {
        for(const auto& uniformBuffer : materialsUniformBuffers)
            VulkanApi::DisposeBuffer(vulkanContext->logicalDevice, uniformBuffer->buffer, uniformBuffer->bufferMemory);
        
        vkDestroyDescriptorSetLayout(vulkanContext->logicalDevice, descriptorSetLayout, nullptr);

        for(auto pool : descriptorPools)
        {
            vkDestroyDescriptorPool(vulkanContext->logicalDevice, pool, nullptr);
        }
        descriptorPools.clear();
    }

    void DescriptorMaterialOpaque::CreateLayout()
    {
        const auto diffuseMap = VulkanApi::DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto specularMap = VulkanApi::DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto normalMap = VulkanApi::DescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto alphaMap = VulkanApi::DescriptorSetLayoutBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto emissiveMap = VulkanApi::DescriptorSetLayoutBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto material = VulkanApi::DescriptorSetLayoutBinding(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        std::vector bindings = { diffuseMap, specularMap, normalMap, alphaMap, emissiveMap, material };
        VulkanApi::CreateDescriptorLayout(vulkanContext->logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorMaterialOpaque::CreateDescriptorSets()
    {
        descriptorSets.resize(VulkanApi::VulkanContext::maxFramesInFlight);
        for(int i = 0; i < VulkanApi::VulkanContext::maxFramesInFlight; i++)
        {
            VulkanApi::AllocateDescriptorSets(vulkanContext->logicalDevice, descriptorSetLayout, 
                descriptorPools.at(i), descriptorSets.at(i), VulkanApi::maxDescriptorSets);
        }

        materialsUniformBuffers.resize(engineContext->resourcesStorage->materialsPaths.size());
        for(int i = 0; i < engineContext->resourcesStorage->materialsPaths.size(); i++)
        {
            materialsUniformBuffers.at(i) = CreateUniformBuffer(vulkanContext, sizeof(VulkanApi::UboMaterial));
        }
    }
    
    void DescriptorMaterialOpaque::UpdateDescriptorSets(const uint16_t frame) const
    {
        Engine::Profiler::GetInstance().BeginSample("Update Opaque Material Descriptors");
        for(int i = 0; i < engineContext->resourcesStorage->materials.size(); i++)
        {
            const auto material = engineContext->resourcesStorage->materials.at(i);
            if (material == nullptr) continue;
            
            VkImageView baseTextuerImageView = nullptr;
            if (material->baseTexture.has_value())
            {
                baseTextuerImageView = engineContext->resourcesStorage->imagesViews.at(material->baseTexture.value());
            }

            VkImageView metallicRoughnessImageView = nullptr;
            if (material->metallicRoughnessTexture.has_value())
            {
                metallicRoughnessImageView = engineContext->resourcesStorage->imagesViews.at(material->metallicRoughnessTexture.value());
            }

            VkImageView normalMapImageView = nullptr;
            if (material->normalsTexture.has_value())
            {
                normalMapImageView = engineContext->resourcesStorage->imagesViews.at(material->normalsTexture.value());
            }

            VkImageView occlusionTextureImageView = nullptr;
            if (material->occlusionTexture.has_value())
            {
                occlusionTextureImageView = engineContext->resourcesStorage->imagesViews.at(material->occlusionTexture.value());
            }

            VkImageView emissionTextureImageView = nullptr;
            if (material->emissiveTexture.has_value())
            {
                emissionTextureImageView = engineContext->resourcesStorage->imagesViews.at(material->emissiveTexture.value());
            }
            
            VkDescriptorImageInfo baseTextureInfo{};
            baseTextureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            baseTextureInfo.imageView = baseTextuerImageView;
            baseTextureInfo.sampler = engineContext->renderer->textureSampler;

            VkDescriptorImageInfo metallicRoughnessInfo{};
            metallicRoughnessInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            metallicRoughnessInfo.imageView = metallicRoughnessImageView;
            metallicRoughnessInfo.sampler = engineContext->renderer->textureSampler;

            VkDescriptorImageInfo normalImageInfo{};
            normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            normalImageInfo.imageView = normalMapImageView;
            normalImageInfo.sampler = engineContext->renderer->textureSampler;

            VkDescriptorImageInfo occlusionInfo{};
            occlusionInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            occlusionInfo.imageView = occlusionTextureImageView;
            occlusionInfo.sampler = engineContext->renderer->textureSampler;

            VkDescriptorImageInfo emissionInfo{};
            emissionInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            emissionInfo.imageView = emissionTextureImageView;
            emissionInfo.sampler = engineContext->renderer->textureSampler;

            VulkanApi::UboMaterial uboMaterial = VulkanApi::UboMaterial(engineContext->resourcesStorage->materials.at(i));
            memcpy(materialsUniformBuffers.at(i)->bufferMapped, &uboMaterial, sizeof(VulkanApi::UboMaterial));
            
            VkDescriptorBufferInfo materialBufferInfo {};
            materialBufferInfo.buffer = materialsUniformBuffers.at(i)->buffer;
            materialBufferInfo.range = sizeof(VulkanApi::UboMaterial);
            materialBufferInfo.offset = 0;
            
            std::array<VkWriteDescriptorSet, 6> descriptorWrites{};

            auto descriptorSet = descriptorSets.at(frame).at(i);

            VulkanApi::WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &baseTextureInfo, nullptr);

            VulkanApi::WriteDescriptorSet(descriptorWrites[1], descriptorSet, 1, 0, 1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &metallicRoughnessInfo, nullptr);

            VulkanApi::WriteDescriptorSet(descriptorWrites[2], descriptorSet, 2, 0, 1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr);

            VulkanApi::WriteDescriptorSet(descriptorWrites[3], descriptorSet, 3, 0, 1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &occlusionInfo, nullptr);

            VulkanApi::WriteDescriptorSet(descriptorWrites[4], descriptorSet, 4, 0, 1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &emissionInfo, nullptr);

            VulkanApi::WriteDescriptorSet(descriptorWrites[5], descriptorSet, 5, 0, 1,
                                          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &materialBufferInfo);
            
            vkUpdateDescriptorSets(vulkanContext->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
        Engine::Profiler::GetInstance().EndSample();
    }
    #pragma optimize("", on)
}
