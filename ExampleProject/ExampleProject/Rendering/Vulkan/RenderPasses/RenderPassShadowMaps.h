#pragma once

#include <GLFW/glfw3.h>

#include "Core/IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

namespace ClientVulkanApi
{
    class RenderPassShadowMaps : public IRenderPass
    {
    public:
        RenderPassShadowMaps(VulkanApi::VulkanContext* vulkanContext, const Ref<RenderPassContext>& renderPassContext);
        ~RenderPassShadowMaps() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;
        void RenderEntity(const DrawEntity& drawEntity, const VkCommandBuffer& commandBuffer, const Ref<PipelineVulkan>& pipeline) const;
        
        Ref<VulkanApi::ImageModel> GetImageBuffer() { return shadowMapBufferModel; }
        VkSampler& GetSampler() { return sampler; }

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
        void CreateShadowMapBuffer();

        void CreateSampler();

        Ref<VulkanApi::ImageModel> shadowMapBufferModel;
        VkSampler sampler;
    };
}
