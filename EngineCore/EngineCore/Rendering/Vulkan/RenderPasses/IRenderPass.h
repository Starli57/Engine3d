#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsAllocator.h"

namespace AVulkan
{
    class DescriptorsManager;

    class IRenderPass
    {
    public:
        IRenderPass(
            VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, const Ref<DescriptorsManager>& descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData)
            : physicalDevice(physicalDevice), logicalDevice(logicalDevice), ecs(ecs), descriptorsManager(descriptorsManager),
                assetsDatabase(assetsDatabase), rendererConfig(rendererConfig), swapChainData(swapChainData)
        {
        }
        virtual ~IRenderPass() = default;
        
        virtual void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) = 0;

        VkRenderPass& GetRenderPass() { return renderPass; }
        VkFramebuffer& GetFrameBuffer() { return frameBuffers.at(0); }

    protected:
        VkPhysicalDevice& physicalDevice;
        VkDevice& logicalDevice;

        Ref<Ecs> ecs;
        Ref<AssetsDatabaseVulkan> assetsDatabase;
        Ref<VulkanConfiguration> rendererConfig;
        Ref<SwapChainData> swapChainData;
        Ref<DescriptorsManager> descriptorsManager;
        
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> frameBuffers;
        
        std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

        virtual void CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig) = 0;
        virtual void CreatePipelines() = 0;
        virtual void CreateFrameBuffers() = 0;

        virtual void BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex) = 0;
    };
}
