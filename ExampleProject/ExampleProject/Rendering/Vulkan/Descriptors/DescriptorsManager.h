#pragma once
#include "DescriptorFrame.h"
#include "DescriptorMaterialOpaque.h"
#include "DescriptorShadowMap.h"
#include "EngineCore/EngineContext.h"
#include "EngineCore/Managers/InputManager.h"

namespace ClientVulkanApi
{
    class DescriptorsManager
    {
    public:
        DescriptorsManager(Engine::EngineContext* engineContext);
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
        Engine::EngineContext* engineContext;
        
        URef<DescriptorFrame> frameDescriptor;
        URef<DescriptorMaterialOpaque> opaqueMaterialDescriptor;
        URef<DescriptorShadowMap> shadowMapDescriptor;
        VkDescriptorPool globalDescriptorPool;
    };
}
