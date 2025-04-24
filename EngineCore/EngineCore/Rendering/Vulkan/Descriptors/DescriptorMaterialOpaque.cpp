#include "EngineCore/Pch.h"
#include "DescriptorMaterialOpaque.h"

#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"

namespace AVulkan
{
    #pragma optimize("", off)
    DescriptorMaterialOpaque::DescriptorMaterialOpaque(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
        const Ref<DescriptorsAllocator>& descriptorsAllocator, VkSampler& textureSampler, const Ref<AssetsDatabaseVulkan>& assetsDatabase)
        : IDescriptor(physicalDevice, logicalDevice, ecs, descriptorsAllocator), textureSampler(textureSampler), assetsDatabase(assetsDatabase)
    {
        descriptorPools.resize(VulkanConfiguration::maxFramesInFlight);
        for(int i = 0; i < VulkanConfiguration::maxFramesInFlight; i++)
        {
            descriptorsAllocator->CreateDescriptorPool(logicalDevice, descriptorPools.at(i), descriptorsAllocator->maxDescriptorSets);
        }
        
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorMaterialOpaque::~DescriptorMaterialOpaque()
    {
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

        std::vector bindings = { diffuseMap, specularMap, normalMap, alphaMap };
        descriptorsAllocator->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorMaterialOpaque::CreateDescriptorSets()
    {
        descriptorSets.resize(VulkanConfiguration::maxFramesInFlight);
        for(int i = 0; i < VulkanConfiguration::maxFramesInFlight; i++)
        {
            descriptorsAllocator->AllocateDescriptorSets(logicalDevice, descriptorSetLayout, 
                descriptorPools.at(i), descriptorSets.at(i), descriptorsAllocator->maxDescriptorSets);
        }
    }
    
    void DescriptorMaterialOpaque::UpdateDescriptorSets(const uint16_t frame) const
    {
        for(int i = 0; i < assetsDatabase->materials.size(); i++)
        {
            const auto material = assetsDatabase->materials.at(i);
            if (material == nullptr) continue;
            
            VkImageView diffuseImageView = nullptr;
            if (material->diffuse.has_value())
            {
                diffuseImageView = assetsDatabase->imagesViews.at(material->diffuse.value());
            }

            VkImageView specularImageView = nullptr;
            if (material->specular.has_value())
            {
                specularImageView = assetsDatabase->imagesViews.at(material->specular.value());
            }

            VkImageView normalMapImageView = nullptr;
            if (material->normalMap.has_value())
            {
                normalMapImageView = assetsDatabase->imagesViews.at(material->normalMap.value());
            }

            VkImageView alphaMapImageView = nullptr;
            if (material->alphaMap.has_value())
            {
                alphaMapImageView = assetsDatabase->imagesViews.at(material->alphaMap.value());
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

            std::array<VkWriteDescriptorSet, 4> descriptorWrites{};

            auto descriptorSet = descriptorSets.at(frame).at(i);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &diffuseImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[1], descriptorSet, 1, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &specularImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[2], descriptorSet, 2, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr);

            descriptorsAllocator->WriteDescriptorSet(descriptorWrites[3], descriptorSet, 3, 0, 1,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &alphaImageInfo, nullptr);

            vkUpdateDescriptorSets(logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }
    #pragma optimize("", on)
}
