#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Core/AssetsDatabase.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
    class IRenderPass
    {
    public:
        IRenderPass(
            VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<VulkanConfiguration> rendererConfig,
            Ref<Ecs> ecs, Ref<AssetsDatabase> assetsDatabase, Ref<SwapChainData> swapChainData, Ref<Descriptors> descriptors)
            : physicalDevice(physicalDevice), logicalDevice(logicalDevice), rendererConfig(rendererConfig),
                ecs(ecs), assetsDatabase(assetsDatabase), swapChainData(swapChainData), descriptors(descriptors)
        {
        }

        virtual ~IRenderPass() = default;
        
        virtual void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) = 0;

        VkRenderPass& GetRenderPass() { return renderPass; }
        VkDescriptorSetLayout& GetDescriptorSetLayout() { return descriptorSetLayout; }
        VkFramebuffer& GetFrameBuffer() { return frameBuffers.at(0); }

    protected:
        VkPhysicalDevice& physicalDevice;
        VkDevice& logicalDevice;

        Ref<Ecs> ecs;
        Ref<AssetsDatabase> assetsDatabase;
        Ref<VulkanConfiguration> rendererConfig;
        Ref<SwapChainData> swapChainData;
        Ref<Descriptors> descriptors;

        std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkFramebuffer> frameBuffers;

        virtual void CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig) = 0;
        virtual void CreateDescriptorLayout(VkDevice& logicalDevice) = 0;
        virtual void CreatePipelines() = 0;
        virtual void CreateFrameBuffers() = 0;

        virtual void BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex) = 0;

        void WriteDescriptorSet(VkWriteDescriptorSet& writeSet, VkDescriptorSet& descriptorSet, uint32_t dstBinding, uint32_t dstArrayElement,
            uint32_t descriptorCount, VkDescriptorType descriptorType, VkDescriptorImageInfo* pImageInfo, VkDescriptorBufferInfo* pBufferInfo);
    };
}