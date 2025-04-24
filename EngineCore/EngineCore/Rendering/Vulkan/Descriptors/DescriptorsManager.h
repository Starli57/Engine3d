#pragma once
#include "DescriptorFrame.h"
#include "DescriptorMaterialOpaque.h"
#include "DescriptorsAllocator.h"
#include "DescriptorShadowMap.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace AVulkan
{
    class DescriptorsManager
    {
    public:
        DescriptorsManager(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
            VkSampler& textureSampler, const Ref<AssetsDatabaseVulkan>& assetsDatabase);
        ~DescriptorsManager();

        void CreateGlobalDescriptorsPool();
        
        void UpdateFrameDescriptors(uint32_t frame) const;
        void UpdateMaterialsDescriptors(uint32_t frame) const;
        void UpdateShadowsMapDescriptors(uint32_t frame, const VkImageView& shadowImageView, const VkSampler& shadowSampler) const;
        
        VkDescriptorSetLayout& GetDescriptorSetLayoutFrame() const;
        VkDescriptorSetLayout& GetDescriptorSetLayoutOpaqueMaterial() const;
        VkDescriptorSetLayout& GetDescriptorSetLayoutShadowMap() const;
        
        VkDescriptorSet& GetDescriptorSetFrame(uint32_t frame) const;
        VkDescriptorSet& GetDescriptorSetOpaqueMaterial(uint32_t frame, uint32_t materialIndex) const;
        VkDescriptorSet& GetDescriptorSetShadowMap(uint32_t frame) const;
        
    private:
        VkDevice& logicalDevice;
        
        Ref<DescriptorsAllocator> descriptorsAllocator;
        Ref<DescriptorFrame> frameDescriptor;
        Ref<DescriptorMaterialOpaque> opaqueMaterialDescriptor;
        Ref<DescriptorShadowMap> shadowMapDescriptor;
        VkDescriptorPool globalDescriptorPool;
    };
}
