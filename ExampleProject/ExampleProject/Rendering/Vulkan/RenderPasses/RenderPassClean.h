#pragma once
#include "IRenderPass.h"

namespace AVulkan
{
    class RenderPassClean : public IRenderPass
    {
    public:
        RenderPassClean(
            Ref<VulkanContext> vulkanContext, const Ref<DescriptorsManager>& descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData);
        ~RenderPassClean() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
    };
}
