#pragma once
#include "Core/IRenderPass.h"
#include "Core/RenderPassContext.h"

namespace ClientVulkanApi
{
    class RenderPassClean : public IRenderPass
    {
    public:
        RenderPassClean(VulkanApi::VulkanContext* vulkanContext,const Ref<RenderPassContext>& renderPassContext);
        ~RenderPassClean() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
    };
}
