#pragma once
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
    class DescriptorShadowMap
    {
    public:
        DescriptorShadowMap(VulkanContext* vulkanContext, const Ref<Ecs>& ecs, const VkDescriptorPool& descriptorPool);

        ~DescriptorShadowMap();
        void CreateLayout();
        void CreateDescriptorSets();
        void UpdateDescriptorSets(uint16_t frame, const VkImageView& shadowImageView, const VkSampler& shadowSampler) const;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet& GetDescriptorSet(const uint32_t frame) { return descriptorSets.at(frame); }

    private:
        VulkanContext* vulkanContext;
        Ref<Ecs> ecs;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    };
}
