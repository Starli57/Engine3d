#include "EngineCore/Pch.h"
#include "DescriptorShadowMap.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/DescriptorsAllocator.h"

namespace ClientVulkanApi
{
    DescriptorShadowMap::DescriptorShadowMap(VulkanApi::VulkanContext* vulkanContext, const Ref<Ecs>& ecs, const VkDescriptorPool& descriptorPool)
        : vulkanContext(vulkanContext), ecs(ecs), descriptorPool(descriptorPool)
    {
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorShadowMap::~DescriptorShadowMap()
    {
        vkDestroyDescriptorSetLayout(vulkanContext->logicalDevice, descriptorSetLayout, nullptr);
    }

    void DescriptorShadowMap::CreateLayout()
    {
        const auto shadowMap = VulkanApi::DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        
        std::vector bindings = { shadowMap };
        VulkanApi::CreateDescriptorLayout(vulkanContext->logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorShadowMap::CreateDescriptorSets()
    {
        descriptorSets.resize(VulkanApi::VulkanContext::maxFramesInFlight);
        VulkanApi::AllocateDescriptorSets(vulkanContext->logicalDevice, descriptorSetLayout, 
                                          descriptorPool, descriptorSets, VulkanApi::VulkanContext::maxFramesInFlight);
    }

    void DescriptorShadowMap::UpdateDescriptorSets(uint16_t frame, const VkImageView& shadowImageView,
        const VkSampler& shadowSampler) const
    {
        Engine::Profiler::GetInstance().BeginSample("Update ShadowMap Descriptors");
        VkDescriptorImageInfo shadowMapInfo{};
        shadowMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        shadowMapInfo.imageView = shadowImageView;
        shadowMapInfo.sampler = shadowSampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        auto descriptorSet = descriptorSets.at(frame);
        VulkanApi::WriteDescriptorSet(descriptorWrites[0], descriptorSet, 0, 0, 1,
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &shadowMapInfo, nullptr);

        vkUpdateDescriptorSets(vulkanContext->logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        Engine::Profiler::GetInstance().EndSample();
    }
}
