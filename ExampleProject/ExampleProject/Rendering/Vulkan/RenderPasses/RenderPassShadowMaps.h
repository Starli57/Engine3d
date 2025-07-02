#pragma once

#include <GLFW/glfw3.h>

#include "IRenderPass.h"
#include "PreRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
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
        RenderPassShadowMaps(Ref<VulkanContext> vulkanContext, Ref<DescriptorsManager> descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData);
        ~RenderPassShadowMaps() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;

        Ref<ImageModel> GetImageBuffer() { return shadowMapBufferModel; }
        VkSampler& GetSampler() { return sampler; }

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
        void CreateShadowMapBuffer();

        void CreateSampler();

        Ref<ImageModel> shadowMapBufferModel;
        VkSampler sampler;
    };
}
