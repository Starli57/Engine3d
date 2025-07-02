#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"

namespace AVulkan
{
    class IRenderPass
    {
    public:
        IRenderPass(
            Ref<VulkanContext> vulkanContext, const Ref<DescriptorsManager>& descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData)
            : vulkanContext(vulkanContext), ecs(ecs), assetsDatabase(assetsDatabase),
                swapChainData(swapChainData), descriptorsManager(descriptorsManager)
        {
            renderPass = VK_NULL_HANDLE;
        }
        virtual ~IRenderPass() = default;
        
        virtual void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) = 0;

        VkRenderPass& GetRenderPass() { return renderPass; }
        VkFramebuffer& GetFrameBuffer() { return frameBuffers.at(0); }

    protected:
        Ref<VulkanContext> vulkanContext;
        Ref<Ecs> ecs;
        Ref<AssetsDatabaseVulkan> assetsDatabase;
        Ref<SwapChainData> swapChainData;
        Ref<DescriptorsManager> descriptorsManager;
        
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> frameBuffers;
        std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

        virtual void CreateRenderPass() = 0;
        virtual void CreatePipelines() = 0;
        virtual void CreateFrameBuffers() = 0;

        void BeginRenderPass(const VkCommandBuffer& commandBuffer, const uint32_t imageIndex, uint32_t colorAttachmentsCount, uint32_t depthAttachmentsCount) const;
    };
}
