#pragma once

#include <GLFW/glfw3.h>

#include "RenderPassContext.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"

namespace VulkanApi
{
    class IRenderPass
    {
    public:
        IRenderPass(
            VulkanContext* vulkanContext, const Ref<RenderPassContext>& renderPassContext)
            : vulkanContext(vulkanContext), renderPassContext(renderPassContext)
        {
            renderPass = VK_NULL_HANDLE;
        }
        virtual ~IRenderPass() = default;
        
        virtual void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) = 0;

        VkRenderPass& GetRenderPass() { return renderPass; }
        VkFramebuffer& GetFrameBuffer() { return frameBuffers.at(0); }

    protected:
        VulkanContext* vulkanContext;
        Ref<RenderPassContext> renderPassContext;
        
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> frameBuffers;
        std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

        virtual void CreateRenderPass() = 0;
        virtual void CreatePipelines() = 0;
        virtual void CreateFrameBuffers() = 0;

        void BeginRenderPass(const VkCommandBuffer& commandBuffer, const uint32_t imageIndex, uint32_t colorAttachmentsCount, uint32_t depthAttachmentsCount) const;
    };
}
