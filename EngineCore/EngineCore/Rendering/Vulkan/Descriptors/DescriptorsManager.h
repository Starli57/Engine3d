#pragma once
#include "DescriptorFrame.h"
#include "DescriptorMaterialOpaque.h"
#include "DescriptorsAllocator.h"
#include "DescriptorShadowMap.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace VulkanApi
{
    class DescriptorsManager
    {
    public:
        DescriptorsManager(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager,
            VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase);
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
        URef<DescriptorFrame> frameDescriptor;
        URef<DescriptorMaterialOpaque> opaqueMaterialDescriptor;
        URef<DescriptorShadowMap> shadowMapDescriptor;
        VkDescriptorPool globalDescriptorPool;

        Ref<Engine::InputManager> inputManager;
    };
}
