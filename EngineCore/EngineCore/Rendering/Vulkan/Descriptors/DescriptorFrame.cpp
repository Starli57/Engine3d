#include "EngineCore/Pch.h"
#include "DescriptorFrame.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboLight.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Utilities/MathUtility.h"

namespace AVulkan
{
    DescriptorFrame::DescriptorFrame(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager,
        VkDescriptorPool& descriptorPool, const Ref<DescriptorsAllocator>& descriptorsAllocator)
        : IDescriptor(physicalDevice, logicalDevice, ecs, descriptorsAllocator), descriptorPool(descriptorPool), inputManager(inputManager)
    {
        CreateLayout();
        CreateDescriptorSets();
    }

    DescriptorFrame::~DescriptorFrame()
    {
        for(const auto& cameraUniform : cameraUniformBuffers)
            VkUtils::DisposeBuffer(logicalDevice, cameraUniform->buffer, cameraUniform->bufferMemory);
        for(const auto& lightUniform : lightUniformBuffers)
            VkUtils::DisposeBuffer(logicalDevice, lightUniform->buffer, lightUniform->bufferMemory);
        for(const auto& cursorUniform : cursorUniformBuffers)
            VkUtils::DisposeBuffer(logicalDevice, cursorUniform->buffer, cursorUniform->bufferMemory);
        
        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
    }

    void DescriptorFrame::CreateLayout()
    {    
        const auto uboViewProjectionLayout = descriptorsAllocator->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        const auto uboLightLayout = descriptorsAllocator->DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        const auto uboCursorLayout = descriptorsAllocator->DescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);

        std::vector bindings = { uboViewProjectionLayout, uboLightLayout, uboCursorLayout };
        descriptorsAllocator->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorFrame::CreateDescriptorSets()
    {
        descriptorsAllocator->AllocateDescriptorSets(logicalDevice, descriptorSetLayout, descriptorPool, descriptorSets, VulkanContext::maxFramesInFlight);

        cameraUniformBuffers.resize(VulkanContext::maxFramesInFlight);
        lightUniformBuffers.resize(VulkanContext::maxFramesInFlight);
        cursorUniformBuffers.resize(VulkanContext::maxFramesInFlight);
        
        for(int i = 0; i < VulkanContext::maxFramesInFlight; i++)
        {
            cameraUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboWorldComponent));
            lightUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboLight));
            cursorUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(glm::vec3));
        }
    }

    void DescriptorFrame::UpdateDescriptorSets(const uint16_t frame) const
    {
        Engine::Profiler::GetInstance().BeginSample("Update Frame Descriptors");
        UpdateUniformBuffer(frame);
        
        VkDescriptorBufferInfo viewProjectionDescriptorInfo{};
        viewProjectionDescriptorInfo.buffer = cameraUniformBuffers.at(frame)->buffer;
        viewProjectionDescriptorInfo.range = sizeof(UboWorldComponent);
        viewProjectionDescriptorInfo.offset = 0;

        VkDescriptorBufferInfo lightDescriptorInfo{};
        lightDescriptorInfo.buffer = lightUniformBuffers.at(frame)->buffer;
        lightDescriptorInfo.range = sizeof(UboLight);
        lightDescriptorInfo.offset = 0;

        VkDescriptorBufferInfo cursorDescriptorInfo{};
        cursorDescriptorInfo.buffer = cursorUniformBuffers.at(frame)->buffer;
        cursorDescriptorInfo.range = sizeof(glm::vec3);
        cursorDescriptorInfo.offset = 0;
        
        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};
        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSets.at(frame), 0, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &viewProjectionDescriptorInfo);

        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[1], descriptorSets.at(frame), 1, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &lightDescriptorInfo);
        
        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[2], descriptorSets.at(frame), 2, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &cursorDescriptorInfo);
        
        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        Engine::Profiler::GetInstance().EndSample();
    }
    
    void DescriptorFrame::UpdateUniformBuffer(uint16_t frame) const
    {
        PositionComponent rendererPosition;
        UboWorldComponent cameraProjection;
        UboWorldComponent lightsProjection;
        UpdateRendererPositionAndProjection(rendererPosition, cameraProjection, lightsProjection);
        
        memcpy(cameraUniformBuffers.at(frame)->bufferMapped, &cameraProjection, sizeof(UboWorldComponent));
        memcpy(cursorUniformBuffers.at(frame)->bufferMapped, &inputManager->cursorPlacementWorldPosition, sizeof(glm::vec3));
        
        //todo: support multiple lights
        const auto lightEntries = ecs->registry->view<PositionComponent, RotationComponent, LightComponent>();
        for (const auto entity : lightEntries)
        {
            //todo: now lights descriptor can handle only 1 light entry, need to extend
            auto& position = lightEntries.get<PositionComponent>(entity).position;
            auto& rotation = lightEntries.get<RotationComponent>(entity).rotation;
            auto& lightComponent = lightEntries.get<LightComponent>(entity);
            glm::mat4 viewProjection = lightsProjection.projection * lightsProjection.view;

            glm::vec3 direction;
            Engine::CalculateDirection(&direction, rotation + glm::vec3(0,90,0));
            Engine::Normalize(&direction);

            UboLight uboLight = UboLight(viewProjection, position, direction, lightComponent.color, lightComponent.intensity);
            memcpy(lightUniformBuffers.at(frame)->bufferMapped, &uboLight, sizeof(UboLight));
        }
    }

    void DescriptorFrame::UpdateRendererPositionAndProjection(PositionComponent& cameraPositionComponent,
        UboWorldComponent& cameraProjection, UboWorldComponent& lightProjection) const
    {
        auto cameraEntries = ecs->registry->view<PositionComponent, CameraComponent, UboWorldComponent>();
        if (cameraEntries.begin() != cameraEntries.end())
        {
            auto firstCamera = cameraEntries.front();
            cameraPositionComponent = cameraEntries.get<PositionComponent>(firstCamera);
            cameraProjection = cameraEntries.get<UboWorldComponent>(firstCamera);
        }

        //todo: support multiple light entries 
        auto lightsEntries = ecs->registry->view<PositionComponent, LightComponent, UboWorldComponent>();
        if (lightsEntries.begin() != lightsEntries.end())
        {
            auto firstLight = lightsEntries.front();
            lightProjection = lightsEntries.get<UboWorldComponent>(firstLight);
        }
    }
}
