#include "EngineCore/Pch.h"
#include "DescriptorFrame.h"

#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboLight.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Utilities/MathUtility.h"

namespace AVulkan
{
    DescriptorFrame::DescriptorFrame(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
        VkDescriptorPool& descriptorPool, const Ref<DescriptorsAllocator>& descriptorsAllocator)
        : IDescriptor(physicalDevice, logicalDevice, ecs, descriptorsAllocator), descriptorPool(descriptorPool)
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
        
        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
    }

    void DescriptorFrame::CreateLayout()
    {    
        const auto uboViewProjectionLayout = descriptorsAllocator->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        const auto uboLightLayout = descriptorsAllocator->DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);

        std::vector bindings = { uboViewProjectionLayout, uboLightLayout };
        descriptorsAllocator->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void DescriptorFrame::CreateDescriptorSets()
    {
        descriptorsAllocator->AllocateDescriptorSets(logicalDevice, descriptorSetLayout, descriptorPool, descriptorSets, VulkanConfiguration::maxFramesInFlight);

        cameraUniformBuffers.resize(VulkanConfiguration::maxFramesInFlight);
        lightUniformBuffers.resize(VulkanConfiguration::maxFramesInFlight);
        for(int i = 0; i < VulkanConfiguration::maxFramesInFlight; i++)
        {
            cameraUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboWorldComponent));
            lightUniformBuffers.at(i) = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboLight));
        }
    }

    void DescriptorFrame::UpdateDescriptorSets(const uint16_t frame)
    {
        UpdateUniformBuffer(frame);
        
        VkDescriptorBufferInfo viewProjectionDescriptorInfo{};
        viewProjectionDescriptorInfo.buffer = cameraUniformBuffers.at(frame)->buffer;
        viewProjectionDescriptorInfo.range = sizeof(UboWorldComponent);
        viewProjectionDescriptorInfo.offset = 0;

        VkDescriptorBufferInfo lightDescriptorInfo{};
        lightDescriptorInfo.buffer = lightUniformBuffers.at(frame)->buffer;
        lightDescriptorInfo.range = sizeof(UboLight);
        lightDescriptorInfo.offset = 0;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[0], descriptorSets.at(frame), 0, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &viewProjectionDescriptorInfo);

        descriptorsAllocator->WriteDescriptorSet(descriptorWrites[1], descriptorSets.at(frame), 1, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &lightDescriptorInfo);
        
        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
    
    void DescriptorFrame::UpdateUniformBuffer(uint16_t frame) const
    {
        PositionComponent rendererPosition;
        UboWorldComponent cameraProjection;
        UboWorldComponent lightsProjection;
        UpdateRendererPositionAndProjection(rendererPosition, cameraProjection, lightsProjection);
        
        memcpy(cameraUniformBuffers.at(frame)->bufferMapped, &cameraProjection, sizeof(UboWorldComponent));

        //todo: support multiple lights
        const auto lightEntries = ecs->registry->view<PositionComponent, RotationComponent, UboDiffuseLightComponent>();
        for (const auto entity : lightEntries)
        {
            //todo: now color descriptor can handle only 1 light entry, need to extend
            auto& position = lightEntries.get<PositionComponent>(entity).position;
            auto& rotation = lightEntries.get<RotationComponent>(entity).rotation;
            glm::mat4 viewProjection = lightsProjection.projection * lightsProjection.view;

            glm::vec3 direction;
            CalculateDirection(&direction, rotation + glm::vec3(0,90,0));
            Normalize(&direction);

            UboLight uboLight = UboLight(viewProjection, position, direction);
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
        auto lightsEntries = ecs->registry->view<PositionComponent, UboDiffuseLightComponent, UboWorldComponent>();
        if (lightsEntries.begin() != lightsEntries.end())
        {
            auto firstLight = lightsEntries.front();
            lightProjection = lightsEntries.get<UboWorldComponent>(firstLight);
        }
    }
}
