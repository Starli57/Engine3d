#include "EngineCore/Pch.h"
#include "IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"

namespace VulkanApi
{
    void IRenderPass::BeginRenderPass(const VkCommandBuffer& commandBuffer, const uint32_t imageIndex, uint32_t colorAttachmentsCount, uint32_t depthAttachmentsCount) const
    {
        std::vector<VkClearValue> clearValues;
        clearValues.resize(colorAttachmentsCount + depthAttachmentsCount);

        for(uint32_t i = 0; i < colorAttachmentsCount; i++)
            clearValues[i].color = { 0.015f, 0.015f, 0.04f, 1.0f };

        for(uint32_t i = 0; i < depthAttachmentsCount; i++)
            clearValues[colorAttachmentsCount + i].depthStencil.depth = 1.0f;

        VulkanApi::BeginRenderPass(clearValues, frameBuffers[imageIndex], renderPass, commandBuffer, renderPassContext->swapChainData->extent);
    }
}
