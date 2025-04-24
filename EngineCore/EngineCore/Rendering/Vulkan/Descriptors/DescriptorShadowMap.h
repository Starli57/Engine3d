#pragma once
#include "IDescriptor.h"

namespace AVulkan
{
    class DescriptorShadowMap : public IDescriptor
    {
    public:
        DescriptorShadowMap(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, VkDescriptorPool& descriptorPool,
            const Ref<DescriptorsAllocator>& descriptorsAllocator);

        ~DescriptorShadowMap() override;
        void CreateLayout() override;
        void CreateDescriptorSets() override;
        void UpdateDescriptorSets(uint16_t frame, const VkImageView& shadowImageView, const VkSampler& shadowSampler) const;

        VkDescriptorSet& GetDescriptorSet(const uint32_t frame) { return descriptorSets.at(frame); }

    private:
        VkDescriptorPool& descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    };
}
