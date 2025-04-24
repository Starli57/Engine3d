#pragma once

#include <GLFW/glfw3.h>

#include "IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/TextureSamplerUtility.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

namespace AVulkan
{
    class RenderPassShadowMaps : public IRenderPass
    {
    public:
        RenderPassShadowMaps(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, Ref<DescriptorsManager> descriptorsManager,
                             const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData,
                             const Ref<PipelinesCollection>& pipelinesCollection);
        ~RenderPassShadowMaps() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) override;

        Ref<ImageModel> GetImageBuffer() { return shadowMapBufferModel; }
        VkSampler& GetSampler() { return sampler; }

    protected:
        void CreateRenderPass(Ref<VulkanConfiguration> rendererConfig) override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
        void CreateShadowMapBuffer();

        void BeginRenderPass(VkCommandBuffer& commandBuffer, uint32_t imageIndex) override;
        void CreateSampler();

        Ref<PipelinesCollection> pipelinesCollection;
        Ref<ImageModel> shadowMapBufferModel;
        VkSampler sampler;
    };
}
