#include "EngineCore/Pch.h"
#include "DescriptorsManager.h"

#include "EngineCore/Rendering/Vulkan/Utilities/DescriptorsAllocator.h"

namespace VulkanApi
{
    DescriptorsManager::DescriptorsManager(VulkanContext* vulkanContext, const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager,
            VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase)
                : vulkanContext(vulkanContext), inputManager(inputManager)
    {
        CreateGlobalDescriptorsPool();
        frameDescriptor = CreateUniqueRef<DescriptorFrame>(vulkanContext, ecs, inputManager, globalDescriptorPool);
        opaqueMaterialDescriptor = CreateUniqueRef<DescriptorMaterialOpaque>(vulkanContext, ecs, textureSampler, assetsDatabase);
        shadowMapDescriptor = CreateUniqueRef<DescriptorShadowMap>(vulkanContext, ecs, globalDescriptorPool);
    }

    DescriptorsManager::~DescriptorsManager()
    {
        shadowMapDescriptor.reset();
        opaqueMaterialDescriptor.reset();
        frameDescriptor.reset();
        vkDestroyDescriptorPool(vulkanContext->logicalDevice, globalDescriptorPool, nullptr);
    }

    void DescriptorsManager::CreateGlobalDescriptorsPool()
    {
        CreateDescriptorPool(vulkanContext->logicalDevice, globalDescriptorPool, maxDescriptorSets);
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
        return frameDescriptor->descriptorSetLayout;
    }

    VkDescriptorSetLayout& DescriptorsManager::GetDescriptorSetLayoutOpaqueMaterial() const
    {
        return opaqueMaterialDescriptor->descriptorSetLayout;
    }

    VkDescriptorSetLayout& DescriptorsManager::GetDescriptorSetLayoutShadowMap() const
    {
        return shadowMapDescriptor->descriptorSetLayout;
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
