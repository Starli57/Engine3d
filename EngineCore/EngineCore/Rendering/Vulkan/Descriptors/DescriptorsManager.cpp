#include "EngineCore/Pch.h"
#include "DescriptorsManager.h"

namespace AVulkan
{
    DescriptorsManager::DescriptorsManager(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager,
            VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase)
                : logicalDevice(logicalDevice), inputManager(inputManager)
    {
        descriptorsAllocator = CreateRef<DescriptorsAllocator>();
        CreateGlobalDescriptorsPool();
        frameDescriptor = CreateUniqueRef<DescriptorFrame>(physicalDevice, logicalDevice, ecs, inputManager, globalDescriptorPool, descriptorsAllocator);
        opaqueMaterialDescriptor = CreateUniqueRef<DescriptorMaterialOpaque>(physicalDevice, logicalDevice, ecs,
            descriptorsAllocator, textureSampler, assetsDatabase);
        shadowMapDescriptor = CreateUniqueRef<DescriptorShadowMap>(physicalDevice, logicalDevice, ecs, globalDescriptorPool, descriptorsAllocator);
    }

    DescriptorsManager::~DescriptorsManager()
    {
        shadowMapDescriptor.reset();
        opaqueMaterialDescriptor.reset();
        frameDescriptor.reset();
        vkDestroyDescriptorPool(logicalDevice, globalDescriptorPool, nullptr);
    }

    void DescriptorsManager::CreateGlobalDescriptorsPool()
    {
        descriptorsAllocator->CreateDescriptorPool(logicalDevice, globalDescriptorPool, descriptorsAllocator->maxDescriptorSets);
    }

    void DescriptorsManager::UpdateFrameDescriptors(const uint32_t frame) const
    {
        frameDescriptor->UpdateDescriptorSets(static_cast<uint16_t>(frame));
    }

    void DescriptorsManager::UpdateMaterialsDescriptors(const uint32_t frame) const
    {
        opaqueMaterialDescriptor->UpdateDescriptorSets(static_cast<uint16_t>(frame));
    }

    void DescriptorsManager::UpdateShadowsMapDescriptors(const uint32_t frame, const VkImageView& shadowImageView, const VkSampler& shadowSampler) const
    {
        shadowMapDescriptor->UpdateDescriptorSets(static_cast<uint16_t>(frame), shadowImageView, shadowSampler);
    }

    VkDescriptorSetLayout& DescriptorsManager::GetDescriptorSetLayoutFrame() const
    {
        return frameDescriptor->DescriptorSetLayout();
    }

    VkDescriptorSetLayout& DescriptorsManager::GetDescriptorSetLayoutOpaqueMaterial() const
    {
        return opaqueMaterialDescriptor->DescriptorSetLayout();
    }

    VkDescriptorSetLayout& DescriptorsManager::GetDescriptorSetLayoutShadowMap() const
    {
        return shadowMapDescriptor->DescriptorSetLayout();
    }

    VkDescriptorSet& DescriptorsManager::GetDescriptorSetFrame(const uint32_t frame) const
    {
        return frameDescriptor->GetDescriptorSet(frame);
    }

    VkDescriptorSet& DescriptorsManager::GetDescriptorSetOpaqueMaterial(const uint32_t frame, const uint32_t materialIndex) const
    {
        return opaqueMaterialDescriptor->GetDescriptorSet(frame, materialIndex);
    }

    VkDescriptorSet& DescriptorsManager::GetDescriptorSetShadowMap(uint32_t frame) const
    {
        return shadowMapDescriptor->GetDescriptorSet(frame);
    }
}
