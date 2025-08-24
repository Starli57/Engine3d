#include "EngineCore/Pch.h"
#include "DescriptorMaterialOpaque.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboMaterial.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"

namespace AVulkan
{
    #pragma optimize("", off)
    DescriptorMaterialOpaque::DescriptorMaterialOpaque(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
        const Ref<DescriptorsAllocator>& descriptorsAllocator, VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& resourcesStorage)
        : IDescriptor(physicalDevice, logicalDevice, ecs, descriptorsAllocator), textureSampler(textureSampler), resourcesStorage(resourcesStorage)
    {
        descriptorPools.resize(VulkanContext::maxFramesInFlight);
        for(int i = 0; i < VulkanContext::maxFramesInFlight; i++)
        {
            descriptorsAllocator->CreateDescriptorPool(logicalDevice, descriptorPools.at(i), descriptorsAllocator->maxDescriptorSets);
        }
        
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorMaterialOpaque::~DescriptorMaterialOpaque()
    {
        for(const auto& uniformBuffer : materialsUniformBuffers)
            VkUtils::DisposeBuffer(logicalDevice, uniformBuffer->buffer, uniformBuffer->bufferMemory);
        
        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

        for(auto pool : descriptorPools)
        {
            vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
        }
        descriptorPools.clear();
    }

    void DescriptorMaterialOpaque::CreateLayout()
    {
        const auto diffuseMap = descriptorsAllocator->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto specularMap = descriptorsAllocator->DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto normalMap = descriptorsAllocator->DescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto alphaMap = descriptorsAllocator->DescriptorSetLayoutBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto emissiveMap = descriptorsAllocator->DescriptorSetLayoutBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        const auto material = descriptorsAllocator->DescriptorSetLayoutBinding(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        std::vector bindings = { diffuseMap, specularMap, normalMap, alphaMap, emissiveMap, material };
        descriptorsAllocator->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorMaterialOpaque::CreateDescriptorSets()
    {
        descriptorSets.resize(VulkanContext::maxFramesInFlight);
        for(int i = 0; i < VulkanContext::maxFramesInFlight; i++)
        {
            descriptorsAllocator->AllocateDescriptorSets(logicalDevice, descriptorSetLayout, 
                descriptorPools.at(i), descriptorSets.at(i), descriptorsAllocator->maxDescriptorSets);
        }

        materialsUniformBuffers.resize(resourcesStorage->materialsPaths.size());
        for(int i = 0; i < resourcesStorage->materialsPaths.size(); i++)
        {
            materialsUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboMaterial));
        }
    }
    
    void DescriptorMaterialOpaque::UpdateDescriptorSets(const uint16_t frame) const
    {
        Engine::Profiler::GetInstance().BeginSample("Update Opaque Material Descriptors");
        for(int i = 0; i < resourcesStorage->materials.size(); i++)
        {
            const auto material = resourcesStorage->materials.at(i);
            if (material == nullptr) continue;
            
            VkImageView baseTextuerImageView = nullptr;
            if (material->baseTexture.has_value())
            {
                baseTextuerImageView = resourcesStorage->imagesViews.at(material->baseTexture.value());
            }

            VkImageView metallicRoughnessImageView = nullptr;
            if (material->metallicRoughnessTexture.has_value())
            {
                metallicRoughnessImageView = resourcesStorage->imagesViews.at(material->metallicRoughnessTexture.value());
            }

            VkImageView normalMapImageView = nullptr;
            if (material->normalsTexture.has_value())
            {
                normalMapImageView = resourcesStorage->imagesViews.at(material->normalsTexture.value());
            }

            VkImageView occlusionTextureImageView = nullptr;
            if (material->occlusionTexture.has_value())
            {
                occlusionTextureImageView = resourcesStorage->imagesViews.at(material->occlusionTexture.value());
            }

            VkImageView emissionTextureImageView = nullptr;
            if (material->emissiveTexture.has_value())
            {
                emissionTextureImageView = resourcesStorage->imagesViews.at(material->emissiveTexture.value());
            }
            
            VkDescriptorImageInfo baseTextureInfo{};
            baseTextureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            baseTextureInfo.imageView = baseTextuerImageView;
            baseTextureInfo.sampler = textureSampler;

            VkDescriptorImageInfo metallicRoughnessInfo{};
            metallicRoughnessInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            metallicRoughnessInfo.imageView = metallicRoughnessImageView;
            metallicRoughnessInfo.sampler = textureSampler;

            VkDescriptorImageInfo normalImageInfo{};
            normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            normalImageInfo.imageView = normalMapImageView;
            normalImageInfo.sampler = textureSampler;

            VkDescriptorImageInfo occlusionInfo{};
            occlusionInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            occlusionInfo.imageView = occlusionTextureImageView;
            occlusionInfo.sampler = textureSampler;

            VkDescriptorImageInfo emissionInfo{};
            emissionInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            emissionInfo.imageView = emissionTextureImageView;
            emissionInfo.sampler = textureSampler;
            
            UboMaterial uboMaterial = UboMaterial(resourcesStorage->materials.at(i));
            memcpy(materialsUniformBuffers.at(i)->bufferMapped, &uboMaterial, sizeof(UboMaterial));
            
            VkDescriptorBufferInfo materialBufferInfo {};
            materialBufferInfo.buffer = materialsUniformBuffers.at(i)->buffer;
            materialBufferInfo.range = sizeof(UboMaterial);
            materialBufferInfo.offset = 0;
            
            std::array<VkWriteDescriptorSet, 6> descriptorWrites{};

            auto descriptorSet = descriptorSets.at(frame).at(i);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &baseTextureInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[1], descriptorSet, 1, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &metallicRoughnessInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[2], descriptorSet, 2, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[3], descriptorSet, 3, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &occlusionInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[4], descriptorSet, 4, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &emissionInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[5], descriptorSet, 5, 0, 1,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &materialBufferInfo);
            
            vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
        Engine::Profiler::GetInstance().EndSample();
    }
    #pragma optimize("", on)
}
