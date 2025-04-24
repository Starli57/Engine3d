#pragma once
#include "IDescriptor.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
    class DescriptorMaterialOpaque : public IDescriptor
    {
    public:
        DescriptorMaterialOpaque(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
            const Ref<DescriptorsAllocator>& descriptorsAllocator, VkSampler& textureSampler, const Ref<AssetsDatabaseVulkan>& assetsDatabase);

        ~DescriptorMaterialOpaque() override;
        void CreateLayout() override;
        void CreateDescriptorSets() override;
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSet& GetDescriptorSet(const uint32_t frame, const uint32_t materialIndex) { return descriptorSets.at(frame).at(materialIndex); }

    private:
        Ref<AssetsDatabaseVulkan> assetsDatabase;
        
        std::vector<VkDescriptorPool> descriptorPools;
        std::vector<std::vector<VkDescriptorSet>> descriptorSets;
        VkSampler& textureSampler;
    };
}
