#pragma once
#include "IDescriptor.h"
#include "DescriptorsAllocator.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Components/LightComponent.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

using namespace EngineCore;
namespace AVulkan
{
    class DescriptorFrame : public IDescriptor
    {
    public:
        DescriptorFrame(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, Ref<InputManager> inputManager,
            VkDescriptorPool& descriptorPool, const Ref<DescriptorsAllocator>& descriptorsAllocator);
        ~DescriptorFrame() override;

        void CreateLayout() override;
        void CreateDescriptorSets() override;
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSet& GetDescriptorSet(const uint32_t frame) { return descriptorSets.at(frame); }
        
    private:
        void UpdateUniformBuffer(uint16_t frame) const;
        void UpdateRendererPositionAndProjection(PositionComponent& cameraPositionComponent, 
            UboWorldComponent& cameraProjection, UboWorldComponent& lightProjection) const;
        
        VkDescriptorPool& descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<Ref<BufferModel>> cameraUniformBuffers;
        std::vector<Ref<BufferModel>> lightUniformBuffers;
        std::vector<Ref<BufferModel>> cursorUniformBuffers;
        
        Ref<InputManager> inputManager;
    };
}
