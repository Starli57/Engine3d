#include "EngineCore/Pch.h"
#include "DescriptorShadowMap.h"

#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"


namespace AVulkan
{
    DescriptorShadowMap::DescriptorShadowMap(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
        const Ref<Ecs>& ecs, VkDescriptorPool& descriptorPool, const Ref<DescriptorsAllocator>& descriptorsAllocator)
        : IDescriptor(physicalDevice, logicalDevice, ecs, descriptorsAllocator), descriptorPool(descriptorPool)
    {
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorShadowMap::~DescriptorShadowMap()
    {
        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
    }

    void DescriptorShadowMap::CreateLayout()
    {
        const auto shadowMap = descriptorsAllocator->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        
        std::vector bindings = { shadowMap };
        descriptorsAllocator->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorShadowMap::CreateDescriptorSets()
    {
        descriptorSets.resize(VulkanConfiguration::maxFramesInFlight);
        descriptorsAllocator->AllocateDescriptorSets(logicalDevice, descriptorSetLayout, 
                descriptorPool, descriptorSets, VulkanConfiguration::maxFramesInFlight);
    }

    void DescriptorShadowMap::UpdateDescriptorSets(uint16_t frame, const VkImageView& shadowImageView,
        const VkSampler& shadowSampler) const
    {
        VkDescriptorImageInfo shadowMapInfo{};
        shadowMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        shadowMapInfo.imageView = shadowImageView;
        shadowMapInfo.sampler = shadowSampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        auto descriptorSet = descriptorSets.at(frame);
        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &shadowMapInfo, nullptr);

        vkUpdateDescriptorSets(logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }
}
