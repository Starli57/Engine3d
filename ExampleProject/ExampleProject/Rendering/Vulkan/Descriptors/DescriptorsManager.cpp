#include "EngineCore/Pch.h"
#include "DescriptorsManager.h"

#include "EngineCore/Rendering/Vulkan/ApiWrappers/DescriptorsAllocator.h"

namespace ClientVulkanApi
{
    DescriptorsManager::DescriptorsManager(Engine::EngineContext* engineContext)
        : engineContext(engineContext)
    {
        CreateGlobalDescriptorsPool();
        frameDescriptor = CreateUniqueRef<DescriptorFrame>(engineContext, globalDescriptorPool);
        opaqueMaterialDescriptor = CreateUniqueRef<DescriptorMaterialOpaque>(engineContext);
        shadowMapDescriptor = CreateUniqueRef<DescriptorShadowMap>(engineContext->renderer->vulkanContext, engineContext->ecs, globalDescriptorPool);
    }

    DescriptorsManager::~DescriptorsManager()
    {
        shadowMapDescriptor.reset();
        opaqueMaterialDescriptor.reset();
        frameDescriptor.reset();
        vkDestroyDescriptorPool(engineContext->renderer->vulkanContext->logicalDevice, globalDescriptorPool, nullptr);
    }

    void DescriptorsManager::CreateGlobalDescriptorsPool()
    {
        VulkanApi::CreateDescriptorPool(engineContext->renderer->vulkanContext->logicalDevice, globalDescriptorPool, VulkanApi::maxDescriptorSets);
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
