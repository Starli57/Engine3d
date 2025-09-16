#pragma once
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace VulkanApi
{
    class DescriptorFrame
    {
    public:
        DescriptorFrame(VulkanContext* vulkanContext, const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager, VkDescriptorPool& descriptorPool);
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

        VulkanContext* vulkanContext;
        Ref<Ecs> ecs;
        VkDescriptorPool descriptorPool;
        
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<Ref<BufferModel>> cameraUniformBuffers;
        std::vector<Ref<BufferModel>> lightUniformBuffers;
        std::vector<Ref<BufferModel>> cursorUniformBuffers;
        
        Ref<Engine::InputManager> inputManager;
    };
}
