#pragma once
#include <GLFW/glfw3.h>
#include "IRenderPass.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

namespace AVulkan
{
    class RenderPassGrid : public IRenderPass
    {
    public:
        RenderPassGrid(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, Ref<DescriptorsManager> descriptorsManager,
                             const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData,
                             const Ref<PipelinesCollection>& pipelinesCollection);
        ~RenderPassGrid() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) override;

    };
}
