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
        const Ref<DescriptorsAllocator>& descriptorsAllocator, VkSampler& textureSampler, const Ref<ResourcesStorageVulkan>& resourcesStorage)
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
        const auto transparency = descriptorsAllocator->DescriptorSetLayoutBinding(4, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        std::vector bindings = { diffuseMap, specularMap, normalMap, alphaMap, transparency };
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
        Profiler::GetInstance().BeginSample("Update Opaque Material Descriptors");
        for(int i = 0; i < resourcesStorage->materials.size(); i++)
        {
            const auto material = resourcesStorage->materials.at(i);
            if (material == nullptr) continue;
            
            VkImageView diffuseImageView = nullptr;
            if (material->diffuse.has_value())
            {
                diffuseImageView = resourcesStorage->imagesViews.at(material->diffuse.value());
            }

            VkImageView specularImageView = nullptr;
            if (material->specular.has_value())
            {
                specularImageView = resourcesStorage->imagesViews.at(material->specular.value());
            }

            VkImageView normalMapImageView = nullptr;
            if (material->normalMap.has_value())
            {
                normalMapImageView = resourcesStorage->imagesViews.at(material->normalMap.value());
            }

            VkImageView alphaMapImageView = nullptr;
            if (material->alphaMap.has_value())
            {
                alphaMapImageView = resourcesStorage->imagesViews.at(material->alphaMap.value());
            }

            VkDescriptorImageInfo diffuseImageInfo{};
            diffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            diffuseImageInfo.imageView = diffuseImageView;
            diffuseImageInfo.sampler = textureSampler;

            VkDescriptorImageInfo specularImageInfo{};
            specularImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            specularImageInfo.imageView = specularImageView;
            specularImageInfo.sampler = textureSampler;

            VkDescriptorImageInfo normalImageInfo{};
            normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            normalImageInfo.imageView = normalMapImageView;
            normalImageInfo.sampler = textureSampler;

            VkDescriptorImageInfo alphaImageInfo{};
            alphaImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            alphaImageInfo.imageView = alphaMapImageView;
            alphaImageInfo.sampler = textureSampler;

            UboMaterial uboMaterial = UboMaterial(resourcesStorage->materials.at(i));
            memcpy(materialsUniformBuffers.at(i)->bufferMapped, &uboMaterial, sizeof(UboMaterial));
            
            VkDescriptorBufferInfo transparencyBufferInfo {};
            transparencyBufferInfo.buffer = materialsUniformBuffers.at(i)->buffer;
            transparencyBufferInfo.range = sizeof(UboMaterial);
            transparencyBufferInfo.offset = 0;
            
            std::array<VkWriteDescriptorSet, 5> descriptorWrites{};

            auto descriptorSet = descriptorSets.at(frame).at(i);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &diffuseImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[1], descriptorSet, 1, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &specularImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[2], descriptorSet, 2, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[3], descriptorSet, 3, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &alphaImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[4], descriptorSet, 4, 0, 1,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &transparencyBufferInfo);
            
            vkUpdateDescriptorSets(logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
        Profiler::GetInstance().EndSample();
    }
    #pragma optimize("", on)
}
