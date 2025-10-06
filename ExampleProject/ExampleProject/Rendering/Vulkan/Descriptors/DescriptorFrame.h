#pragma once
#include "EngineCore/EngineContext.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace ClientVulkanApi
{
    class DescriptorFrame
    {
    public:
        DescriptorFrame(Engine::EngineContext* engineContext, VkDescriptorPool& descriptorPool);
        ~DescriptorFrame();

        void CreateLayout();
        void CreateDescriptorSets();
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet& GetDescriptorSet(const uint32_t frame) { return descriptorSets.at(frame); }
        
    private:
        void UpdateUniformBuffer(uint16_t frame) const;
        void UpdateRendererPositionAndProjection(PositionComponent& cameraPositionComponent, 
            UboWorldComponent& cameraProjection, UboWorldComponent& lightProjection) const;

        Engine::EngineContext* engineContext;
        VulkanApi::VulkanContext* vulkanContext;
        
        VkDescriptorPool descriptorPool;
        
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<Ref<VulkanApi::BufferModel>> cameraUniformBuffers;
        std::vector<Ref<VulkanApi::BufferModel>> lightUniformBuffers;
        std::vector<Ref<VulkanApi::BufferModel>> cursorUniformBuffers;
    };
}
